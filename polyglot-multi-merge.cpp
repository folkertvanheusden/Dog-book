#include <cinttypes>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <arpa/inet.h>

typedef struct {
	uint64_t key;
	uint16_t move;
	uint16_t weight;
	uint16_t n;
	uint16_t sum;
} entry_t;

#define HTONLL(x) ((1==htonl(1)) ? (x) : (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
#define NTOHLL(x) (HTONLL(x))

int main(int argc, char *argv[])
{
	uint64_t                 min_n = 0;
	std::string              out;
	std::vector<std::string> files;
	std::map<uint64_t, std::map<uint16_t, uint64_t> > work;

	for(int i=1; i<argc; i++) {
		if (strcmp(argv[i], "-in") == 0)
			files.push_back(argv[++i]);
		else if (strcmp(argv[i], "-out") == 0)
			out = argv[++i];
		else if (strcmp(argv[i], "-min-n") == 0)
			min_n = std::stoll(argv[++i]);
		else
			printf("\"%s\" is not understood\n", argv[i]);
	}

	if (out.empty())
		out = "out.bin";

	for(auto & file: files) {
		printf("Working on %s\n", file.c_str());
		std::ifstream book_file(file, std::ios::binary);
		if (book_file.good() == false) {
			fprintf(stderr, "Cannot open file\n");
			return 1;
		}

		uint64_t count = 0;
		entry_t  entry;
		while(book_file.read(reinterpret_cast<char *>(&entry), sizeof entry)) {
			uint64_t hash = HTONLL(entry.key);

			auto it = work.find(hash);
			if (it == work.end())
				work.insert({ hash, { { htons(entry.move), 1 } } });
			else {
				auto mit = it->second.find(htons(entry.move));
				if (mit == it->second.end())
					it->second.insert({ htons(entry.move), 1 });
				else
					mit->second++;
			}

			count++;
		}

		printf("Read %" PRIu64 " entries from book\n", count);
	}

	// check counts
	uint64_t max_count   = 0;
	uint64_t total_count = 0;
	uint64_t total_sq    = 0;
	uint64_t n = 0;
	for(auto & e: work) {
		for(auto &me: e.second) {
			max_count = std::max(max_count, me.second);
			total_count += me.second;
			total_sq += me.second * me.second;
			n++;
		}
	}

	printf("Max. count: %" PRIu64 "\n", max_count);
	double avg = total_count / double(n);
	printf("Avg. count: %.2f\n", avg);
	printf("Std. dev. : %.2f\n", sqrt(total_sq / double(n) - avg * avg));
	if (max_count > 65535) {
		uint64_t scale = (max_count + 32766) / 65532;
		printf("Dividing by %" PRIu64 "\n", scale);
		for(auto & e: work) {
			for(auto &me: e.second)
				me.second /= scale;
		}
	}

	// write
	uint64_t      skipped = 0;
	std::ofstream output(out, std::ios::binary);
	for(auto & e: work) {
		for(auto &me: e.second) {
			if (me.second >= min_n) {
				entry_t e_out { };
				e_out.key    = NTOHLL(e.first);
				e_out.move   = ntohs(me.first);
				e_out.weight = ntohs(me.second);

				output.write(reinterpret_cast<const char *>(&e_out), sizeof e_out);
			}
			else {
				skipped++;
			}
		}
	}
	printf("Total number of moves: %" PRIu64 ", written: %" PRIu64 "\n", n, n - skipped);

	return 0;
}
