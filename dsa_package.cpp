#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <list>
#include <climits>
#include <thread>
#include <chrono>
using namespace std;

class Graph {
    unordered_map<string, list<pair<string, int>>> adj;

public:
    void addEdge(string u, string v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void dijkstra(string src,
                  unordered_map<string, int> &dist,
                  unordered_map<string, string> &parent) {

        priority_queue<
            pair<int, string>,
            vector<pair<int, string>>,
            greater<pair<int, string>>
        > pq;

        for (auto x : adj) {
            dist[x.first] = INT_MAX;
            parent[x.first] = "";
        }

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto top = pq.top(); pq.pop();

            int d = top.first;
            string node = top.second;

            for (auto neighbor : adj[node]) {
                string next = neighbor.first;
                int weight = neighbor.second;

                if (d + weight < dist[next]) {
                    dist[next] = d + weight;
                    parent[next] = node;
                    pq.push({dist[next], next});
                }
            }
        }
    }

    vector<string> getPath(string dest,
                           unordered_map<string, string> &parent) {
        vector<string> path;

        while (dest != "") {
            path.push_back(dest);
            dest = parent[dest];
        }

        reverse(path.begin(), path.end());
        return path;
    }

    void simulateMovement(vector<string> path) {
        for (auto p : path) {
            cout << "Reached: " << p << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
};

int main() {
    Graph g;

    // Graph
    g.addEdge("A", "B", 5);
    g.addEdge("A", "C", 2);
    g.addEdge("B", "D", 3);
    g.addEdge("C", "D", 4);
    g.addEdge("C", "E", 6);
    g.addEdge("D", "E", 1);

    // Data
    string user = "A";
    string restaurant = "D";

    vector<pair<string, string>> deliveryBoys = {
        {"DB1", "B"},
        {"DB2", "E"},
        {"DB3", "C"}
    };

    unordered_map<string, int> dist;
    unordered_map<string, string> parent;

    // Step 1: Find nearest delivery boy to restaurant
    g.dijkstra(restaurant, dist, parent);

    string assignedBoy;
    string boyLocation;
    int minDist = INT_MAX;

    for (auto db : deliveryBoys) {
        if (dist[db.second] < minDist) {
            minDist = dist[db.second];
            assignedBoy = db.first;
            boyLocation = db.second;
        }
    }

    cout << "Assigned Delivery Boy: " << assignedBoy << endl;

    // Step 2: Path from delivery boy → restaurant
    g.dijkstra(boyLocation, dist, parent);
    vector<string> pathToRestaurant = g.getPath(restaurant, parent);

    // Step 3: Path from restaurant → user
    g.dijkstra(restaurant, dist, parent);
    vector<string> pathToUser = g.getPath(user, parent);

    // STATUS UPDATES
    cout << "\nOrder Status: Preparing \n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "Order Status: Picked by " << assignedBoy << "\n";

    cout << "\nMoving to Restaurant:\n";
    g.simulateMovement(pathToRestaurant);

    cout << "\nOrder Picked! Heading to User\n";

    cout << "\nDelivery Route:\n";
    g.simulateMovement(pathToUser);

    cout << "\nOrder Delivered Successfully!\n";

    return 0;
}
