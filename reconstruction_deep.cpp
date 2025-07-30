#include <bits/stdc++.h>
using namespace std;

struct Level {
    long long size = 0;
    int count = 0;
};

struct Order {
    char side;
    double price;
    long long size;
};
map<long long, Order> orders; // Track orders by order_id

template<typename MapIt>
void output_levels(MapIt it_begin, MapIt it_end, ostream &out, char side_char) {
    int printed = 0;
    for (auto it = it_begin; it != it_end && printed < 10; ++it, ++printed) {
        double price = it->first;
        Level lvl = it->second;
        out << price << "," << lvl.size << "," << lvl.count;
        if (printed < 9) out << ",";
    }
    while (printed < 10) {
        out << ",,,";
        printed++;
        if (printed < 10) out << ",";
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <mbo.csv> <output.csv>\n";
        return 1;
    }

    ifstream infile(argv[1]);
    ofstream outfile(argv[2]);
    if (!infile || !outfile) {
        cerr << "File open error." << endl;
        return 1;
    }

    string header;
    getline(infile, header);
    outfile << header << ",depth,price,size,flags,ts_in_delta,sequence,";
    for (int i = 0; i < 10; ++i) outfile << "bid_px_0" << i << ",bid_sz_0" << i << ",bid_ct_0" << i << ",ask_px_0" << i << ",ask_sz_0" << i << ",ask_ct_0" << i << (i < 9 ? "," : "");
    outfile << ",symbol,order_id\n";

    map<double, Level, greater<double>> bids;
    map<double, Level> asks;
    long long skipOrderId = -1;
    string line;

    while (getline(infile, line)) {
        vector<string> f;
        stringstream ss(line);
        string s;
        while (getline(ss, s, ',')) f.push_back(s);
        if (f.size() < 15) continue;

        char action = f[5][0], side = f[6][0];
        double price = f[7].empty() ? 0.0 : stod(f[7]);
        long long size = stoll(f[8]), order_id = stoll(f[10]);

        int depth = 0;
        map<double, Level> *sideMap = nullptr;
        if (side == 'B') sideMap = static_cast<map<double, Level>*>(&bids);
        else if (side == 'A') sideMap = &asks;

        if (action == 'R') {
            bids.clear(); asks.clear();
        } else if (action == 'A' && sideMap) {
            (*sideMap)[price].size += size;
            (*sideMap)[price].count++;
            orders[order_id] = {side, price, size}; // Track new order
        } else if (action == 'C') {
            if (order_id == skipOrderId) {
                skipOrderId = -1;
                continue;
            }
            if (sideMap && sideMap->count(price)) {
                (*sideMap)[price].size -= size;
                (*sideMap)[price].count--;
                if ((*sideMap)[price].count <= 0) sideMap->erase(price);
            }
            orders.erase(order_id); // Remove canceled order
        } else if (action == 'T') {
            if (side != 'N') {
                map<double, Level> *oppMap = (side == 'B') ? &asks : static_cast<map<double, Level>*>(&bids);
                if (oppMap->count(price)) {
                    (*oppMap)[price].size -= size;
                    (*oppMap)[price].count--;
                    if ((*oppMap)[price].count <= 0) oppMap->erase(price);
                }
            }
            skipOrderId = order_id;
        } else if (action == 'F') {
            skipOrderId = order_id;
            continue;
        }

        outfile << line << "," << depth << "," << price << "," << size << "," << f[11] << "," << f[12] << "," << f[13] << ",";
        output_levels(bids.begin(), bids.end(), outfile, 'B'); outfile << ",";
        output_levels(asks.begin(), asks.end(), outfile, 'A');
        outfile << "," << f[14] << "," << ((action == 'T') ? 0 : order_id) << "\n";
    }
    return 0;
}
