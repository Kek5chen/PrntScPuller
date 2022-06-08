#include <iostream>
#include "httplib.h"
#include "parser.h"
#include "generator.h"

using std::cout, std::endl;

const httplib::Headers headers{
		{"User-Agent",      "Mozilla/5.0 (X11; Linux x86_64; rv:101.0) Gecko/20100101 Firefox/101.0"},
		{"Accept",          "image/avif,image/webp,*/*"},
		{"Accept-Encoding", "gzip, deflate, br"},
		{"Accept-Language", "en-US,en;q=0.5"},
		{"Connection",      "keep-alive"},
		{"Sec-Fetch-Dest",  "image"},
		{"Sec-Fetch-Mode",  "cors"}
};

int main() {
	std::string genUrl, path;
	httplib::Client cli("https://prnt.sc");

	for (int i = 0; i < 5; i++) {
		genUrl = generate_new_image_url();
		path = genUrl.substr(genUrl.find_last_of('/'));
		auto result = cli.Get(path.c_str());
		if (!result) { continue; }

		std::string body = result->body;
		std::string url = parse_image_url(body);
		if (url.empty()) {
			cout << "Could not find image url" << endl;
			i--;
			continue;
		} else {
			cout << "Image url: " << url << endl;
		}

		const std::string &imgHost = url.substr(0, 8 + url.substr(8).find_first_of('/'));
		const std::string &imgPath = url.substr(8 + url.substr(8).find_first_of('/'));
		const std::string &imgName = imgPath.substr(imgPath.find_last_of('/') + 1);
		cout << "Image host: " << imgHost << endl;
		cout << "Image sub: " << imgPath << endl;
		cout << "Image name: " << imgName << endl;
		httplib::Client cli2(imgHost);

		cli2.set_default_headers(headers);
		result = cli2.Get(imgPath.c_str());
		if (result && !result->body.empty()) {
			std::ofstream out(imgName.c_str(), std::ios::binary);
			out << result->body;
			out.close();
			cout << "Successfully downloaded: " + url << endl;
		} else {
			cout << "Could not download image" << endl;
			i--;
		}
	}
	return 0;
}
