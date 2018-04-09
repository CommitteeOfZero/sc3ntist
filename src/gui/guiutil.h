#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iomanip>

// https://stackoverflow.com/a/9907752
std::string htmlEscape(const std::string& src) {
  std::stringstream dst;
  for (char ch : src) {
    switch (ch) {
      case '&':
        dst << "&amp;";
        break;
      case '\'':
        dst << "&apos;";
        break;
      case '"':
        dst << "&quot;";
        break;
      case '<':
        dst << "&lt;";
        break;
      case '>':
        dst << "&gt;";
        break;
      default:
        dst << ch;
        break;
    }
  }
  return dst.str();
}

std::string uint8_vector_to_hex_string(const std::vector<uint8_t>& v) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  std::vector<uint8_t>::const_iterator it;

  for (it = v.begin(); it != v.end(); it++) {
    if (it != v.begin()) ss << " ";
    ss << std::setw(2) << static_cast<unsigned>(*it);
  }

  return ss.str();
}