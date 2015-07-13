#pragma once

#include "C:\Work\Enseed_svn\trunk\Applications\Mediatheque\win32\Mediatek\RESTSchema\schema\generated\Class.h"
#include "boost/property_tree/ptree.hpp"
#include "Introspection.h"
#include "boost/optional.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/date_time/posix_time/time_parsers.hpp"
#include "boost/uuid/random_generator.hpp"

class JSONType
{
public:
	enum
	{
		SIMPLE,
		COMPLEX,
		TIME,
		UUID
	};
};

template<typename T, int IS_SIMPLE>
struct TJSONSerializer
{};


template<typename T, int JSON_TYPE = JSONTrait<T>::type>
class JSONSerializer : public TJSONSerializer < T, JSON_TYPE >
{};

template<typename T>
struct TJSONSerializer < T, JSONType::SIMPLE >
{
public:
	void toJSON(const T &obj, boost::property_tree::ptree *rootTree, const std::string &name)
	{
		if (name.empty())
		{
			// array element
			boost::property_tree::ptree elemTree;
			elemTree.put<T>("", obj);
			rootTree->push_back(std::make_pair("", elemTree));
		}
		else
		{
			rootTree->put<T>(name, obj);
		}
	}

	void fromJSON(const boost::property_tree::ptree &treeElem, T *obj)
	{
		std::string value = treeElem.get_value<std::string>();
		*obj = boost::lexical_cast<T>(value);
	}

};



template<class T> struct JSONTrait { static const int type = JSONType::COMPLEX; };

template<> struct JSONTrait < char > { static const int type = JSONType::SIMPLE; };
template<> struct JSONTrait < bool > { static const int type = JSONType::SIMPLE; };
template<> struct JSONTrait < short > { static const int type = JSONType::SIMPLE; };
template<> struct JSONTrait < int > { static const int type = JSONType::SIMPLE; };
template<> struct JSONTrait < long > { static const int type = JSONType::SIMPLE; };
template<> struct JSONTrait < long long > { static const int type = JSONType::SIMPLE; };
template<> struct JSONTrait < float > { static const int type = JSONType::SIMPLE; };
template<> struct JSONTrait < double > { static const int type = JSONType::SIMPLE; };
template<> struct JSONTrait < std::string > { static const int type = JSONType::SIMPLE; };

template<> struct JSONTrait < boost::posix_time::ptime > { static const int type = JSONType::TIME; };

template<> struct JSONTrait < boost::uuids::uuid > { static const int type = JSONType::UUID; };


template<class U>
class JSONVectorDeserializer_InsertionStrategy
{
public:
	static void fromJSON(const boost::property_tree::ptree &ptree, std::vector<U> &vect)
	{
		auto iter = vect.begin();
		for (auto &elem : ptree)
		{
			JSONSerializer<U>().fromJSON(elem.second, &*iter++);
		}
	}
};

template<>
class JSONVectorDeserializer_InsertionStrategy<bool>
{
public:
	static void fromJSON(const boost::property_tree::ptree &ptree, std::vector<bool> &vect)
	{
		int i = 0;
		for (auto &elem : ptree)
		{
			bool val;
			JSONSerializer<bool>().fromJSON(elem.second, &val);
			vect[i++] = val;
		}
	}
};


template<class M, int KIND=M::kind, int IS_REF=M::is_ref, int IS_OPTIONAL=M::is_optional>
struct TJSONSerializeMember
{};

template<class M>
struct TJSONSerializeMember < M, Kind::SIMPLE, false, false >
{
public:
	template<class T>
	static void serialize(const T &obj, boost::property_tree::ptree *tree, const std::string &name)
	{
		JSONSerializer<M::type>().toJSON(*M::readPtr(&obj), tree, M::name());
	}
};

template<class M>
struct TJSONSerializeMember < M, Kind::SIMPLE, false, true>
{
public:
	template<class T>
	static void serialize(const T &obj, boost::property_tree::ptree *tree, const std::string &name)
	{
		auto &optionalValue = *M::readPtr(&obj);
		if (optionalValue)
		{
			JSONSerializer<M::value_type>().toJSON(optionalValue.get(), tree, M::name());
		}
	}
};

template<class M>
struct TJSONSerializeMember < M, Kind::SIMPLE, true, false>
{
public:
	template<class T>
	static void serialize(const T &obj, boost::property_tree::ptree *tree, const std::string &name)
	{
		auto &counPtrValue = *M::readPtr(&obj);
		if (counPtrValue)
		{
			JSONSerializer<M::value_type>().toJSON(*counPtrValue, tree, M::name());
		}
	}
};

template<class M>
struct TJSONSerializeMember<M, Kind::MAP, true, true>
{
public:
	template<class T>
	static void serialize(const T &obj, boost::property_tree::ptree *tree, const std::string &name)
	{
		boost::property_tree::ptree complex;

		//static_assert(typeid(M::key_type) == typeid(std::string));


		boost::property_tree::ptree mapTree;
		auto &opt = *M::readPtr(&obj);
		if (!opt)
			return;

		auto &map = opt.get();
		for (auto &elem : map)
			JSONSerializer<M::value_type>().toJSON(*elem.second.ptr(), &mapTree, elem.first);

		tree->push_back(std::make_pair(name, mapTree));
	}
};


template<class M>
struct TJSONSerializeMember < M, Kind::MAP, false, true>
{
public:
	template<class T>
	static void serialize(const T &obj, boost::property_tree::ptree *tree, const std::string &name)
	{
		boost::property_tree::ptree complex;

		//static_assert(typeid(M::key_type) == typeid(std::string));


		boost::property_tree::ptree mapTree;

		auto &opt = *M::readPtr(&obj);
		if (!opt)
			return;

		auto &map = opt.get();
		for (auto &elem : map)
			JSONSerializer<M::value_type>().toJSON(elem.second, &mapTree, elem.first);

		tree->push_back(std::make_pair(name, mapTree));
	}
};


template<class M>
struct TJSONSerializeMember < M, Kind::MAP, true, false >
{
public:
	template<class T>
	static void serialize(const T &obj, boost::property_tree::ptree *tree, const std::string &name)
	{
		boost::property_tree::ptree complex;

		//static_assert(typeid(M::key_type) == typeid(std::string));


		boost::property_tree::ptree mapTree;
		auto &map = *M::readPtr(&obj);
		for (auto &elem : map)
			JSONSerializer<M::value_type>().toJSON(*elem.second.ptr(), &mapTree, elem.first);

		tree->push_back(std::make_pair(name, mapTree));
	}
};

template<class M>
struct TJSONSerializeMember < M, Kind::MAP, false, false >
{
public:
	template<class T>
	static void serialize(const T &obj, boost::property_tree::ptree *tree, const std::string &name)
	{
		boost::property_tree::ptree complex;

		//static_assert(typeid(M::key_type) == typeid(std::string));

		boost::property_tree::ptree mapTree;
		auto &map = *M::readPtr(&obj);
		for (auto &elem : map)
			JSONSerializer<M::value_type>().toJSON(elem.second, &mapTree, elem.first);

		tree->push_back(std::make_pair(name, mapTree));
	}
};

template<class M>
struct TJSONSerializeMember < M, Kind::LIST, true, true>
{
private:
	typedef typename M::type Type;

public:
	template<class T>
	static void serialize(const T &obj, boost::property_tree::ptree *tree, const std::string &name)
	{
		boost::property_tree::ptree listTree;
		auto *memberValue = M::readPtr(&obj);
		auto &opt = *static_cast<const boost::optional<std::vector<seed::CountPtr<Type>>>*>(memberValue);
		if (!opt)
			return;

		const std::vector<seed::CountPtr<Type>> &vect = opt.get();
		for (auto &elem : vect)
			JSONSerializer<Type>().toJSON(*elem.ptr(), listTree, "");

		_tree.push_back(std::make_pair(_member.name(), listTree));
		tree->push_back(std::make_pair(name, complex));
	}
};

template<class M>
struct TJSONSerializeMember < M, Kind::LIST, false, true>
{
private:
	typedef typename M::type Type;

public:
	template<class T>
	static void serialize(const T &obj, boost::property_tree::ptree *tree, const std::string &name)
	{
		boost::property_tree::ptree listTree;
		auto &opt = *M::readPtr(&obj);
		if (!opt)
			return;

		auto &vect = opt.get();
		for (const M::value_type& elem : vect)
			JSONSerializer<M::value_type>().toJSON(elem, &listTree, "");

		tree->push_back(std::make_pair(name, listTree));
	}
};

template<class M>
struct TJSONSerializeMember < M, Kind::LIST, true, false>
{
private:
	typedef typename M::type Type;

public:
	template<class T>
	static void serialize(const T &obj, boost::property_tree::ptree *tree, const std::string &name)
	{
		boost::property_tree::ptree listTree;
		auto *memberValue = M::readPtr(&obj);
		const std::vector< seed::CountPtr<Type> > &vect = *static_cast<const std::vector< seed::CountPtr<Type> >*>(memberValue);
		for (auto &elem : vect)
			JSONSerializer<Type>().toJSON(*elem.ptr(), listTree, "");

		_tree.push_back(std::make_pair(_member.name(), listTree));

		tree->push_back(std::make_pair(name, complex));
	}
};

template<class M>
struct TJSONSerializeMember < M, Kind::LIST, false, false >
{
public:
	template<class T>
	static void serialize(const T &obj, boost::property_tree::ptree *tree, const std::string &name)
	{
		boost::property_tree::ptree listTree;
		auto *memberValue = M::readPtr(&obj);
		auto &vect = *memberValue;
		for (const M::value_type &elem : vect)
			JSONSerializer<M::value_type>().toJSON(elem, &listTree, "");

		tree->push_back(std::make_pair(name, listTree));
	}
};

template<class T>
struct JSONSerializeMember
{
public:
	const T &obj;
	boost::property_tree::ptree *complex;

	JSONSerializeMember(const T &obj, boost::property_tree::ptree *complex)
		: obj(obj)
		, complex(complex)
	{}

	template<class M>
	void operator()(const M& memberType)
	{
		TJSONSerializeMember<M>::serialize(obj, complex, M::name());
	}
};

template<class M, int KIND = M::kind, int IS_REF=M::is_ref, int IS_OPTIONAL=M::is_optional>
struct JSONDeserializeMember_Specific
{
	template<class T>
	static void deserialize(const boost::property_tree::ptree &treeElem, T *obj)
	{
		auto *member = M::writePtr(obj);
		JSONSerializer<M::value_type>().fromJSON(treeElem, member);
	}
};

template<class M>
struct JSONDeserializeMember_Specific<M, Kind::MAP, true, true>
{
public:
	template<class T>
	static void deserialize(const boost::property_tree::ptree &treeElem, T *obj)
	{
		//assert(keyType == typeid(std::string));
		auto &opt = *M::writePtr(obj);
		opt.emplace();
		auto &map = opt.get();

		for (auto &elem : _tree)
		{
			seed::CountPtr<M::value_type> keyRef(new M::value_type());
			JSONSerializer<M::value_type>().fromJSON(elem.second, keyRef.ptr());
			map[elem.first] = keyRef;
		}
	}
};

template<class M>
struct JSONDeserializeMember_Specific < M, Kind::MAP, false, true >
{
public:
	template<class T>
	static void deserialize(const boost::property_tree::ptree &treeElem, T *obj)
	{
		//assert(keyType == typeid(std::string));
		auto &opt = *M::writePtr(obj);
		opt.emplace();
		auto &map = opt.get();

		for (auto &elem : _tree)
		{
			JSONSerializer<M::value_type>().fromJSON(elem.second, &map[elem.first]);
		}
	}
};


template<class M>
struct JSONDeserializeMember_Specific < M, Kind::MAP, true, false>
{
public:
	template<class T>
	static void deserialize(const boost::property_tree::ptree &treeElem, T *obj)
	{
		//assert(keyType == typeid(std::string));
		auto &map = *M::writePtr(obj);
		for (auto &elem : _tree)
		{
			seed::CountPtr<M::value_type> keyRef(new M::value_type());
			JSONSerializer<M::value_type>().fromJSON(elem.second, keyRef.ptr());
			map[elem.first] = keyRef;
		}
	}
};


template<class M>
struct JSONDeserializeMember_Specific < M, Kind::MAP, false, false >
{
public:
	template<class T>
	static void deserialize(const boost::property_tree::ptree &treeElem, T *obj)
	{
		//assert(keyType == typeid(std::string));
		auto &map = *M::writePtr(obj);
		for (auto &elem : treeElem)
		{
			JSONSerializer<M::value_type>().fromJSON(elem.second, &map[elem.first]);
		}
	}
};


template<class M>
struct JSONDeserializeMember_Specific < M, Kind::LIST, true, true>
{
public:
	template<class T>
	static void deserialize(const boost::property_tree::ptree &treeElem, T *obj)
	{
		auto &opt = *M::writePtr(obj);
		opt.emplace(treeElem.size());
		auto &vect = opt.get();

		auto vecIter = vect.begin();
		for (auto &elem : treeElem)
		{
			seed::CountPtr<M::value_type> keyRef(new M::value_type());
			JSONSerializer<M::value_type>().fromJSON(elem.second, keyRef.ptr());
			*vecIter++ = keyRef;
		}
	}
};



template<class M>
struct JSONDeserializeMember_Specific < M, Kind::LIST, false, true >
{
public:
	template<class T>
	static void deserialize(const boost::property_tree::ptree &treeElem, T *obj)
	{
		auto &opt = *M::writePtr(obj);
		opt.emplace(treeElem.size());
		auto &vect = opt.get();

		// we use a vector inserter because the stl has a special implementation for vector<bool> where
		// bits are packed. For this reason, we insert bool and other types differently.
		JSONVectorDeserializer_InsertionStrategy<U>::fromJSON(_tree, vect);
	}
};



template<class M>
struct JSONDeserializeMember_Specific < M, Kind::LIST, true, false >
{
public:
	template<class T>
	static void deserialize(const boost::property_tree::ptree &treeElem, T *obj)
	{
		auto &vect = *M::writePtr(obj);
		vect.resize(treeElem.size());

		auto vecIter = vect.begin();
		for (auto &elem : treeElem)
		{
			seed::CountPtr<M::value_type> keyRef(new M::value_type());
			JSONSerializer<M::value_type>().fromJSON(elem.second, keyRef.ptr());
			*vecIter++ = keyRef;
		}
	}
};




template<class M>
struct JSONDeserializeMember_Specific < M, Kind::LIST, false, false >
{
public:
	template<class T>
	static void deserialize(const boost::property_tree::ptree &treeElem, T *obj)
	{
		auto &vect = *M::writePtr(obj);
		vect.resize(_tree.size());

		// we use a vector inserter because the stl has a special implementation for vector<bool> where
		// bits are packed. For this reason, we insert bool and other types differently.
		JSONVectorDeserializer_InsertionStrategy<U>::fromJSON(_tree, vect);
	}
};


template<class T>
struct JSONDeserializeMember
{
public:
	T *obj;
	const boost::property_tree::ptree &tree;

	JSONDeserializeMember(const boost::property_tree::ptree &complex, T *obj)
		: obj(obj)
		, tree(complex)
	{}

	template<class M>
	void operator()(const M& memberType)
	{
		auto complex = tree.get_child_optional(M::name());
		if (!complex)
			return;

		JSONDeserializeMember_Specific<M>::deserialize(complex.get(), obj);
	}
};

template<typename T>
struct TJSONSerializer < T, JSONType::COMPLEX >
{
public:
	void toJSON(const T &obj, boost::property_tree::ptree *rootTree, const std::string &name)
	{
		boost::property_tree::ptree objTree;
		JSONSerializeMember<T> memberVisitor(obj, &objTree);
		Class<T>::visitMembers(memberVisitor);
		rootTree->push_back(std::make_pair(name, objTree));
	}

	void fromJSON(const boost::property_tree::ptree &elem, T *obj)
	{
		JSONDeserializeMember<T> memberVisitor(elem, obj);
		Class<T>::visitMembers(memberVisitor);
	}

};

template<typename T>
struct TJSONSerializer < T, JSONType::TIME >
{
public:
	void toJSON(const T &obj, boost::property_tree::ptree *rootTree, const std::string &name)
	{
		std::string value = boost::posix_time::to_iso_extended_string(obj);
		if (name.empty())
		{
			// array element
			boost::property_tree::ptree elemTree;
			elemTree.put<std::string>("", value);
			rootTree->push_back(std::make_pair("", elemTree));
		}
		else
		{
			rootTree->put<std::string>(name, value);
		}
	}

	void fromJSON(const boost::property_tree::ptree &treeElem, T *obj)
	{
		std::string value = treeElem.get_value<std::string>();
		*obj = boost::posix_time::time_from_string(value);
	}
};

template<typename T>
struct TJSONSerializer < T, JSONType::UUID >
{
public:
	void toJSON(const T &obj, boost::property_tree::ptree *rootTree, const std::string &name)
	{
		std::string value = boost::lexical_cast<std::string>(obj);
		if (name.empty())
		{
			// array element
			boost::property_tree::ptree elemTree;
			elemTree.put<std::string>("", value);
			rootTree->push_back(std::make_pair("", elemTree));
		}
		else
		{
			rootTree->put<std::string>(name, value);
		}
	}

	void fromJSON(const boost::property_tree::ptree &rootTree, T *obj)
	{
		std::string value = rootTree.get_value<std::string>();
		*obj = boost::lexical_cast<boost::uuids::uuid>(value);
	}
};

template<class T>
void DeserializeFromType(const std::string &name, boost::property_tree::ptree &tree, T *out)
{
	auto child = tree.get_child_optional(name);
	if (child)
	{
		JSONSerializer<T>().fromJSON(child.get(), out);
	}
}

class JSONParser
{
public:
	template<class T>
	static std::string toJSON(const T &obj)
	{
		boost::property_tree::ptree rootTree;
		JSONSerializer<T>().toJSON(obj, &rootTree, "d");

		std::stringstream strm;
		boost::property_tree::write_json(strm, rootTree);
		return strm.str();
	}

	template<class T>
	static void fromJSON(const std::istream &in, T *out)
	{
		try
		{
			boost::property_tree::ptree rootTree;
			boost::property_tree::read_json(const_cast<std::istream&>(in), rootTree);
			DeserializeFromType("d", rootTree, out);
		}
		catch (const std::exception &ex)
		{
			//std::istream &in2 = const_cast<std::istream&>(in);
			//in2.seekg(0);

			//std::string content{ std::istreambuf_iterator<char>(in2), std::istreambuf_iterator<char>() };
			//throw HttpClientErrorInvalidFormatException(ex, content);
			throw HttpClientErrorInvalidFormatException(ex);
		}
	}
};
