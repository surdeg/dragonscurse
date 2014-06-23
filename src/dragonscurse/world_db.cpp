#include <string>
#include <string.h>
#include "world_db.h"

struct Lock {
    int m_id;
    WorldDB::KeyType m_type;
    std::string m_location;
};

bool WorldDB::load_attributes(Lock *lock, TiXmlElement *elmt)
{
    bool result = true;

    TiXmlAttribute *attr = elmt->FirstAttribute();
    while (attr) {
        if (strcmp(attr->Name(), "id") == 0) {
            lock->m_id = atoi(attr->Value());
        }
        else if (strcmp(attr->Name(), "type") == 0) {
            if (strcmp(attr->Value(), "green") == 0) {
                lock->m_type = KeyTypeGreen;
            }
            else if (strcmp(attr->Value(), "red") == 0) {
                lock->m_type = KeyTypeRed;
            }
            else {
                result = false;
                break;
            }
        }
        else if (strcmp(attr->Name(), "location") == 0) {
            lock->m_location = std::string(attr->Value());
        }
        else {
            result = false;
            break;
        }

        attr = attr->Next();
    }

    return result;
}

bool WorldDB::load_nodes(TiXmlNode *node)
{
    int result = true;

    if (node->Type() == TiXmlNode::TINYXML_ELEMENT) {
        if (strcmp(node->Value(), "lock") == 0) {
            Lock *lock = new Lock;
            result = load_attributes(lock, node->ToElement());
            m_locks.push_back(lock);
        }
    }

    for (TiXmlNode *child = node->FirstChild();
             child != 0;
             child = child->NextSibling()) {
        if (!load_nodes(child)) {
            result = false;
        }
    }

    return result;
}

WorldDB::WorldDB(const char *name)
{
    TiXmlDocument doc(name);
    if (doc.LoadFile()) {
        load_nodes(&doc);
    }
}

