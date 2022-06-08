#pragma once
#include <string>
#include <cstring>

const char* img_search_string = R"(class="no-click screenshot-image" src=")";

inline std::string parse_image_url(const std::string& body) {
	// tokenize body and find the image url
	std::string url;
	size_t pos = body.find(img_search_string);
	if(pos != std::string::npos) {
		pos += strlen(img_search_string);
		size_t end = body.find('\"', pos);
		if(end != std::string::npos) {
			url = body.substr(pos, end - pos);
		}
	}
	return url;
}