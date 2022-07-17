#include <vector>
#include <string>
#include <iostream> 
#include <stdexcept>

using std::string;
using std::vector;
using std::logic_error;
using std::ostream;

namespace ariel {
    
    class OrgChartLevelIterator;

    class OrgChart {
        public:
            OrgChart();
            OrgChart& add_root(string root_role);
            OrgChart& add_sub(string superior, string subordinate);
            struct Node {
                string role; //this will save the node role
                vector<Node*> subordinates;
            };
            Node* root;
            friend ostream &operator<<(ostream& os, const OrgChart& oc);
            OrgChartLevelIterator begin_level_order();
            OrgChartLevelIterator end_level_order();

        private:
            //auxiliary function: 
            Node* find(Node* node, string role);
    };
    ostream &operator<<(ostream& os, const OrgChart& oc);
    
    class OrgChartLevelIterator {
        public:
            OrgChartLevelIterator(OrgChart::Node* root);
            bool operator!=(const OrgChartLevelIterator& iter) const;
            OrgChartLevelIterator& operator++();
            string operator*();
        private:
            vector<OrgChart::Node*> queue;
    };
};