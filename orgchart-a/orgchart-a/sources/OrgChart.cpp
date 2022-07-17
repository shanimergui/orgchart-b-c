#include "../OrgChart.hpp"

using namespace ariel;

OrgChart::OrgChart() {
    this->root = nullptr;
}

OrgChart& OrgChart::add_root(string root_role) {
    // Creating the new root *node* with the role.
    
    // add the node to OrgChart if the root is null
    if (this->root == nullptr) {
        this->root = new Node();
        this->root->role = root_role;
    } // else, only change the role
    else { 
        this->root->role = root_role;
    }
    return *this;
}

OrgChart& OrgChart::add_sub(string superior, string subordinate) {
    //find the superior node
    Node* superior_node = this->find(this->root, superior);

    // throw exception if no superior node found.
    if (superior_node == nullptr) {
        throw logic_error("Cant find superior to add the subordinate to it.");
    }

    // now we know that the superior exists. we are creating the subordinate node
    OrgChart::Node* subordinate_node = new OrgChart::Node();
    subordinate_node->role = subordinate;

    superior_node->subordinates.push_back(subordinate_node);
    return *this;
}

std::ostream& ariel::operator<<(std::ostream& os, const OrgChart& oc) {
    vector<OrgChart::Node*> queue;
    if (oc.root == nullptr) {
        return os;
    }
    queue.push_back(oc.root);
    queue.push_back(nullptr);
    while (!queue.empty()) {
        OrgChart::Node* current = queue.front();
        if (current == nullptr){
            os << std::endl;
            if (queue.size() > 1) {
                queue.push_back(nullptr);
            }
        }
        queue.erase(queue.begin());
        if (current != nullptr) {
            os << current->role << " ";
            for (auto sub: current->subordinates) {
                queue.push_back(sub);
            }
        }
    }
    return os;
}

OrgChart::Node* OrgChart::find(OrgChart::Node* node,string role) {
    if (node == nullptr) 
        return nullptr;

    if (node->role == role) {
        return node;
    }
    for (Node* sub: node->subordinates) {
        Node* result = this->find(sub, role);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

OrgChartLevelIterator OrgChart::begin_level_order() {
    return OrgChartLevelIterator(this->root);
}

OrgChartLevelIterator OrgChart::end_level_order() {
    return OrgChartLevelIterator(nullptr);
}


/***
 * Level Iterator:
*/


OrgChartLevelIterator::OrgChartLevelIterator(OrgChart::Node* root) {
    if (root != nullptr) {
        this->queue.push_back(root);
    }
}

OrgChartLevelIterator& OrgChartLevelIterator::operator++() {
    for (auto sub: this->queue[0]->subordinates) {
        this->queue.push_back(sub);
    }
    this->queue.erase(this->queue.begin());
    return *this;
}

bool OrgChartLevelIterator::operator!=(const OrgChartLevelIterator& iter) const {
    if (this->queue.size() != iter.queue.size()){
        return false;
    }
    for (size_t index =0; index < this->queue.size(); index++) {
        if (this->queue[index] != iter.queue[index]) {
            return false;
        }
    }
    return true;
}

string OrgChartLevelIterator::operator*() {
    return this->queue[0]->role;
}


