#pragma once
#include <test.hpp>
#include <math.h>
#include <yaml-cpp/yaml.h>

Eigen::Vector3d to_eigen(const std::vector<double>& vec) {
  Eigen::Vector3d e_vec;
  for (int i = 0; i < 3; ++i) {
    if (vec[i] == 6.28) {
      e_vec(i) = 2 * M_PI;
    } else {
      e_vec(i) = vec[i];
    }
  }
  return e_vec;
}

bool BBS3DTest::load_config(const std::string& config) {
  YAML::Node conf = YAML::LoadFile(config);

  std::cout << "[YAML] Loading Paths" << std::endl;
  tar_path = conf["target_clouds"].as<std::string>();
  src_path = conf["source_clouds"].as<std::string>();
  output_path = conf["output_folder"].as<std::string>();

  std::cout << "[YAML] Loading 3D-BBS Parameters" << std::endl;
  min_level_res = conf["min_level_res"].as<double>();
  max_level = conf["max_level"].as<int>();

  if (min_level_res == 0.0 || max_level == 0) {
    std::cout << "[ERROR] Set min_level and num_layers except for 0" << std::endl;
    return false;
  }

  std::cout << "[YAML] Loading Angular Search Range" << std::endl;
  std::vector<double> min_rpy_temp = conf["min_rpy"].as<std::vector<double>>();
  std::vector<double> max_rpy_temp = conf["max_rpy"].as<std::vector<double>>();
  if (min_rpy_temp.size() == 3 && max_rpy_temp.size() == 3) {
    min_rpy = to_eigen(min_rpy_temp);
    max_rpy = to_eigen(max_rpy_temp);
  } else {
    std::cout << "[ERROR] Set min_rpy and max_rpy correctly" << std::endl;
    return false;
  }

  std::cout << "[YAML] Downample Souce Clouds Parameters" << std::endl;
  valid_tar_vgf = conf["valid_tar_vgf"].as<bool>();
  if (valid_tar_vgf) tar_leaf_size = conf["tar_leaf_size"].as<float>();

  valid_src_vgf = conf["valid_src_vgf"].as<bool>();
  if (valid_src_vgf) src_leaf_size = conf["src_leaf_size"].as<float>();

  cut_src_points = conf["cut_src_points"].as<bool>();
  if (cut_src_points) {
    scan_range = conf["scan_range"].as<std::pair<double, double>>();
    if (scan_range.second == 0.0) {
      std::cout << "[ERROR] Set max scan_range except for 0" << std::endl;
      return false;
    }
  }

  use_gicp = conf["use_gicp"].as<bool>();
  return true;
}