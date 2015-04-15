#include "mq_util.hpp"

namespace dds {
namespace mq {

std::string type_name_to_topic(std::string type_name) {
    size_t result;

    while ((result = type_name.find("::")) != std::string::npos) {
        type_name.replace(result, 2, "_");
    }

    return type_name;
}

}
}
