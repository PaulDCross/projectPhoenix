/* Auto-generated by genmsg_cpp for file /home/jamie/projectPhoenix/Lib/rosspawn/srv/NodeAction.srv */
#ifndef ROSSPAWN_SERVICE_NODEACTION_H
#define ROSSPAWN_SERVICE_NODEACTION_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"

#include "ros/service_traits.h"




namespace rosspawn
{
template <class ContainerAllocator>
struct NodeActionRequest_ {
  typedef NodeActionRequest_<ContainerAllocator> Type;

  NodeActionRequest_()
  : node_file_name()
  {
  }

  NodeActionRequest_(const ContainerAllocator& _alloc)
  : node_file_name(_alloc)
  {
  }

  typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _node_file_name_type;
  std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  node_file_name;


private:
  static const char* __s_getDataType_() { return "rosspawn/NodeActionRequest"; }
public:
  ROS_DEPRECATED static const std::string __s_getDataType() { return __s_getDataType_(); }

  ROS_DEPRECATED const std::string __getDataType() const { return __s_getDataType_(); }

private:
  static const char* __s_getMD5Sum_() { return "014964366824ce564884dfdaff1faba6"; }
public:
  ROS_DEPRECATED static const std::string __s_getMD5Sum() { return __s_getMD5Sum_(); }

  ROS_DEPRECATED const std::string __getMD5Sum() const { return __s_getMD5Sum_(); }

private:
  static const char* __s_getServerMD5Sum_() { return "014964366824ce564884dfdaff1faba6"; }
public:
  ROS_DEPRECATED static const std::string __s_getServerMD5Sum() { return __s_getServerMD5Sum_(); }

  ROS_DEPRECATED const std::string __getServerMD5Sum() const { return __s_getServerMD5Sum_(); }

private:
  static const char* __s_getMessageDefinition_() { return "string node_file_name\n\
\n\
"; }
public:
  ROS_DEPRECATED static const std::string __s_getMessageDefinition() { return __s_getMessageDefinition_(); }

  ROS_DEPRECATED const std::string __getMessageDefinition() const { return __s_getMessageDefinition_(); }

  ROS_DEPRECATED virtual uint8_t *serialize(uint8_t *write_ptr, uint32_t seq) const
  {
    ros::serialization::OStream stream(write_ptr, 1000000000);
    ros::serialization::serialize(stream, node_file_name);
    return stream.getData();
  }

  ROS_DEPRECATED virtual uint8_t *deserialize(uint8_t *read_ptr)
  {
    ros::serialization::IStream stream(read_ptr, 1000000000);
    ros::serialization::deserialize(stream, node_file_name);
    return stream.getData();
  }

  ROS_DEPRECATED virtual uint32_t serializationLength() const
  {
    uint32_t size = 0;
    size += ros::serialization::serializationLength(node_file_name);
    return size;
  }

  typedef boost::shared_ptr< ::rosspawn::NodeActionRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::rosspawn::NodeActionRequest_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct NodeActionRequest
typedef  ::rosspawn::NodeActionRequest_<std::allocator<void> > NodeActionRequest;

typedef boost::shared_ptr< ::rosspawn::NodeActionRequest> NodeActionRequestPtr;
typedef boost::shared_ptr< ::rosspawn::NodeActionRequest const> NodeActionRequestConstPtr;


template <class ContainerAllocator>
struct NodeActionResponse_ {
  typedef NodeActionResponse_<ContainerAllocator> Type;

  NodeActionResponse_()
  {
  }

  NodeActionResponse_(const ContainerAllocator& _alloc)
  {
  }


private:
  static const char* __s_getDataType_() { return "rosspawn/NodeActionResponse"; }
public:
  ROS_DEPRECATED static const std::string __s_getDataType() { return __s_getDataType_(); }

  ROS_DEPRECATED const std::string __getDataType() const { return __s_getDataType_(); }

private:
  static const char* __s_getMD5Sum_() { return "d41d8cd98f00b204e9800998ecf8427e"; }
public:
  ROS_DEPRECATED static const std::string __s_getMD5Sum() { return __s_getMD5Sum_(); }

  ROS_DEPRECATED const std::string __getMD5Sum() const { return __s_getMD5Sum_(); }

private:
  static const char* __s_getServerMD5Sum_() { return "014964366824ce564884dfdaff1faba6"; }
public:
  ROS_DEPRECATED static const std::string __s_getServerMD5Sum() { return __s_getServerMD5Sum_(); }

  ROS_DEPRECATED const std::string __getServerMD5Sum() const { return __s_getServerMD5Sum_(); }

private:
  static const char* __s_getMessageDefinition_() { return "\n\
\n\
"; }
public:
  ROS_DEPRECATED static const std::string __s_getMessageDefinition() { return __s_getMessageDefinition_(); }

  ROS_DEPRECATED const std::string __getMessageDefinition() const { return __s_getMessageDefinition_(); }

  ROS_DEPRECATED virtual uint8_t *serialize(uint8_t *write_ptr, uint32_t seq) const
  {
    ros::serialization::OStream stream(write_ptr, 1000000000);
    return stream.getData();
  }

  ROS_DEPRECATED virtual uint8_t *deserialize(uint8_t *read_ptr)
  {
    ros::serialization::IStream stream(read_ptr, 1000000000);
    return stream.getData();
  }

  ROS_DEPRECATED virtual uint32_t serializationLength() const
  {
    uint32_t size = 0;
    return size;
  }

  typedef boost::shared_ptr< ::rosspawn::NodeActionResponse_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::rosspawn::NodeActionResponse_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct NodeActionResponse
typedef  ::rosspawn::NodeActionResponse_<std::allocator<void> > NodeActionResponse;

typedef boost::shared_ptr< ::rosspawn::NodeActionResponse> NodeActionResponsePtr;
typedef boost::shared_ptr< ::rosspawn::NodeActionResponse const> NodeActionResponseConstPtr;

struct NodeAction
{

typedef NodeActionRequest Request;
typedef NodeActionResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;
}; // struct NodeAction
} // namespace rosspawn

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::rosspawn::NodeActionRequest_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::rosspawn::NodeActionRequest_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::rosspawn::NodeActionRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "014964366824ce564884dfdaff1faba6";
  }

  static const char* value(const  ::rosspawn::NodeActionRequest_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x014964366824ce56ULL;
  static const uint64_t static_value2 = 0x4884dfdaff1faba6ULL;
};

template<class ContainerAllocator>
struct DataType< ::rosspawn::NodeActionRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "rosspawn/NodeActionRequest";
  }

  static const char* value(const  ::rosspawn::NodeActionRequest_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::rosspawn::NodeActionRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "string node_file_name\n\
\n\
";
  }

  static const char* value(const  ::rosspawn::NodeActionRequest_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros


namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::rosspawn::NodeActionResponse_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::rosspawn::NodeActionResponse_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::rosspawn::NodeActionResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "d41d8cd98f00b204e9800998ecf8427e";
  }

  static const char* value(const  ::rosspawn::NodeActionResponse_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xd41d8cd98f00b204ULL;
  static const uint64_t static_value2 = 0xe9800998ecf8427eULL;
};

template<class ContainerAllocator>
struct DataType< ::rosspawn::NodeActionResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "rosspawn/NodeActionResponse";
  }

  static const char* value(const  ::rosspawn::NodeActionResponse_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::rosspawn::NodeActionResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "\n\
\n\
";
  }

  static const char* value(const  ::rosspawn::NodeActionResponse_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::rosspawn::NodeActionResponse_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::rosspawn::NodeActionRequest_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.node_file_name);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct NodeActionRequest_
} // namespace serialization
} // namespace ros


namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::rosspawn::NodeActionResponse_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct NodeActionResponse_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace service_traits
{
template<>
struct MD5Sum<rosspawn::NodeAction> {
  static const char* value() 
  {
    return "014964366824ce564884dfdaff1faba6";
  }

  static const char* value(const rosspawn::NodeAction&) { return value(); } 
};

template<>
struct DataType<rosspawn::NodeAction> {
  static const char* value() 
  {
    return "rosspawn/NodeAction";
  }

  static const char* value(const rosspawn::NodeAction&) { return value(); } 
};

template<class ContainerAllocator>
struct MD5Sum<rosspawn::NodeActionRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "014964366824ce564884dfdaff1faba6";
  }

  static const char* value(const rosspawn::NodeActionRequest_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct DataType<rosspawn::NodeActionRequest_<ContainerAllocator> > {
  static const char* value() 
  {
    return "rosspawn/NodeAction";
  }

  static const char* value(const rosspawn::NodeActionRequest_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct MD5Sum<rosspawn::NodeActionResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "014964366824ce564884dfdaff1faba6";
  }

  static const char* value(const rosspawn::NodeActionResponse_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct DataType<rosspawn::NodeActionResponse_<ContainerAllocator> > {
  static const char* value() 
  {
    return "rosspawn/NodeAction";
  }

  static const char* value(const rosspawn::NodeActionResponse_<ContainerAllocator> &) { return value(); } 
};

} // namespace service_traits
} // namespace ros

#endif // ROSSPAWN_SERVICE_NODEACTION_H

