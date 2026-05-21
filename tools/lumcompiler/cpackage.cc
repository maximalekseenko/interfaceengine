// Copyright 2026 maxim (necromax) alekseenko

#include "lumcompiler/cpackage.h"

#include <SDL3/SDL_render.h>
#include <necroutils/colexc.h>

#include <cstdio>
#include <fstream>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "interface/lumen/package/exceptions.h"
#include "interface/lumen/package/package_header.h"
#include "lumcompiler/exceptions.h"

namespace lumcompiler {

template <typename T>
void WriteContainer(std::ofstream& out_file, const T& container) {
  out_file.write(container.data(), container.size());
}

template <typename T>
void WriteCasted(std::ofstream& out_file, const T& content) {
  out_file.write(reinterpret_cast<const char*>(&content), sizeof(content));
}

void GetParentPathAndName(const std::string& path, std::string& out_parent_path,
                          std::string& out_name) {
  if (path.empty()) {
    out_parent_path = ".";
    out_name = "";
    return;
  }

  size_t last_slash = path.find_last_of("/\\");

  if (last_slash == std::string::npos) {
    out_parent_path = ".";
    out_name = path;
  } else {
    out_parent_path
        = (last_slash == 0) ? path.substr(0, 1) : path.substr(0, last_slash);
    out_name = path.substr(last_slash + 1);
  }

  size_t last_dot = out_name.find_last_of('.');
  if (last_dot != std::string::npos && last_dot != 0)
    out_name = out_name.substr(0, last_dot);
}

void LumCompilingPackage::CompileLumFile(const std::string& lumdata_path,
                                         std::string lum_out_path,
                                         SDL_Renderer* renderer) {
  std::ifstream lumdata_file;
  std::ofstream lum_file;
  std::string assets_folder, package_name;
  std::set<std::string> required_file_names;
  std::vector<std::string> valid_meta_lines;

  // Open input (lumdata) file.
  lumdata_file.open(lumdata_path, std::ios::binary);
  if (!lumdata_file.is_open())
    throw MissingLumdataError("Cannot compile lumdata file \"" + lumdata_path
                              + "\": File does not exist.");

  GetParentPathAndName(lumdata_path, assets_folder, package_name);

  // Fix lum out path if needed.
  if (!lum_out_path.ends_with(".lum"))
    lum_out_path += '/' + package_name + ".lum";

  // Open output (lum) file.
  lum_file.open(lum_out_path, std::ios::binary);
  if (!lum_file.is_open())
    throw LumcompilerError("Cannot compile lumdata file \"" + lumdata_path
                           + "\": Cannot create output file \"" + lum_out_path
                           + "\".");

  // Load objects from lumfile.
  // This gathers two things:
  // - what lines are legit (valid_meta_lines)
  // - what files had been accessed (required_file_names)
  CollectiveException col_exc("LumPackage had finished loading lumdata file \""
                              + lumdata_path + "\" with following exceptions:");

  int line_index = 0;
  for (std::string lumdata_line; std::getline(lumdata_file, lumdata_line);
       ++line_index) {
    try {
      AddMeta(
          lumdata_line,
          [&required_file_names, &assets_folder](const std::string& file_name) {
            required_file_names.emplace(file_name);
            return std::make_unique<std::ifstream>(assets_folder + '/'
                                                   + file_name);
          },
          renderer);
      valid_meta_lines.push_back(lumdata_line);
    } catch (const lumen::package::MetaEmtpyLine& e) {
    } catch (const lumen::package::MetaCommentedLine& e) {
    } catch (const std::exception& e) {
      col_exc.Add(std::make_exception_ptr(InvalidLumdataError(
          "Failed to process line " + std::to_string(line_index) + " due to:\n"
          + e.what())));
    } catch (...) {
      col_exc.Add(std::make_exception_ptr(InvalidLumdataError(
          "Failed to process line " + std::to_string(line_index) + " due to:\n"
          + "Unknown error.")));
    }
  }

  // Check for errors.
  if (!col_exc.empty()) {
    if (std::remove(lum_out_path.c_str()) != 0)
      col_exc.Add(std::make_exception_ptr(
          LumcompilerError("Failed to clean output lum file \"" + lum_out_path
                           + "\" after error.")));
    throw col_exc;
  }

  // Write header.
  lumen::package::PackageHeader header{
      .magic{'L', 'U', 'M', '0'},
      .version = 0,
      .files_count = required_file_names.size(),
      .meta_lines_count = valid_meta_lines.size()};

  WriteContainer(lum_file, header.magic);
  WriteCasted(lum_file, header.version);
  WriteCasted(lum_file, header.files_count);
  WriteCasted(lum_file, header.meta_lines_count);

  // Write files.
  for (const auto& file_name : required_file_names) {
    std::ifstream file_stream(assets_folder + '/' + file_name);
    lumen::package::PackageHeader::FileNameSize file_name_size;
    lumen::package::PackageHeader::FileSize file_size;

    file_name_size = file_name.size();

    file_stream.seekg(0, std::ios::end);
    file_size = static_cast<uint32_t>(file_stream.tellg());
    file_stream.seekg(0, std::ios::beg);

    WriteCasted(lum_file, file_name_size);
    WriteContainer(lum_file, file_name);
    WriteCasted(lum_file, file_size);

    std::vector<char> buffer(64 * 1024);
    while (file_stream) {
      file_stream.read(buffer.data(), buffer.size());
      std::streamsize n = file_stream.gcount();
      lum_file.write(buffer.data(), n);
    }
  }

  // Write meta.
  for (const auto& meta_line : valid_meta_lines) {
    lumen::package::PackageHeader::MetaSize meta_line_size;

    meta_line_size = meta_line.size();

    WriteCasted(lum_file, meta_line_size);
    WriteContainer(lum_file, meta_line);
  }
}

}  // namespace lumcompiler
