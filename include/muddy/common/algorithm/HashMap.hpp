#ifndef MUDDY_COMMON_ALGORITHM_HASH_TABLE_HPP_INCLUDED_
#define MUDDY_COMMON_ALGORITHM_HASH_TABLE_HPP_INCLUDED_

#include <cstddef>
#include <muddy/common/helper.hpp>
#include <muddy/common/algorithm/Math.hpp>
#include <muddy/common/crypto/hash.hpp>

namespace muddy {

template<typename Key, typename Value = void,
		typename Algorithm = crypto::MurmurHash,
		typename ListNode = helper::ListNode<Key, Value>,
		typename HashValue = helper::HashValue<Key, Algorithm>,
		typename Compare = helper::Compare<Key> >
class HashMap {
public:
	typedef ListNode Node;

	class Cursor {
	public:
		Cursor(size_t s, const Node* const* t)
				: cursor(NULL), tableSize(s), slot(0), listTable(t) {}

		bool next();

		const Node& getNode() const {
			return *cursor;
		}

	private:
		const Node* cursor;
		size_t tableSize, slot;
		const Node* const* listTable;

		bool incSlot();
	};

	explicit HashMap(size_t size = kMinSize);
	~HashMap();

	Node& insert(const Key& key);
	Node* search(const Key& key) const;
	bool remove(const Key& key);

	Cursor getCursor() const {
		return Cursor(tableSize, listTable);
	}

private:
	enum { kMinSize = 13 };

	size_t tableSize;
	size_t nodeCount;
	Node** listTable;

	static void destroy(Node* head);

	void resize(size_t newSize);
};

#define FULL_TMPL HashMap<Key, Value, Algorithm, ListNode, HashValue, \
		Compare>
#define HASH_MAP(...) \
		template<typename Key, \
		typename Value, typename Algorithm, typename ListNode, \
		typename HashValue, typename Compare> \
		\
		__VA_ARGS__ FULL_TMPL

HASH_MAP(bool)::Cursor::next() {
	if (cursor == NULL) {
		return incSlot();
	}
	cursor = cursor->next;
	if (cursor != NULL) {
		return true;
	}
	++slot;
	return incSlot();
};

HASH_MAP(bool)::Cursor::incSlot() {
	while (slot < tableSize && listTable[slot] == NULL) {
		++slot;
	}
	if (slot >= tableSize) {
		return false;
	}
	cursor = listTable[slot];
	return true;
}

HASH_MAP()::HashMap(size_t size) : nodeCount(0) {
	if (size < kMinSize) {
		size = kMinSize;
	}
	if (Math::isPrimeNumber(size)) {
		tableSize = size;
	} else {
		tableSize = Math::getNextPrime(size);
	}
	listTable = new Node*[tableSize];
	for (size_t i = 0; i < tableSize; ++i) {
		listTable[i] = NULL;
	}
}

HASH_MAP()::~HashMap() {
	for (size_t i = 0; i < tableSize; ++i) {
		Node* n = listTable[i];
		while (n != NULL) {
			Node* t = n->next;
			delete n;
			n = t;
		}
	}
	delete[] listTable;
}

HASH_MAP(typename FULL_TMPL::Node&)::insert(const Key& key) {
	uint32_t hash = HashValue::hash(key);
	size_t slot = hash % tableSize;
	Node* ptr = listTable[slot];
	while (ptr != NULL) {
		if (Compare::equal(ptr->key, key)) {
			return *ptr;
		}
		ptr = ptr->next;
	}

	Node* node = new Node(key);
	node->next = listTable[slot];
	listTable[slot] = node;
	nodeCount++;
	if (nodeCount * 4 > tableSize * 3) {
		resize(Math::getNextPrime(tableSize * 2));
	}
	return *node;
}

HASH_MAP(typename FULL_TMPL::Node*)::search(const Key& key) const {
	Node* ptr = listTable[HashValue::hash(key) % tableSize];
	while (ptr != NULL) {
		if (Compare::equal(ptr->key, key)) {
			return ptr;
		}
		ptr = ptr->next;
	}
	return NULL;
}

HASH_MAP(bool)::remove(const Key& key) {
	size_t slot = HashValue::hash(key) % tableSize;
	Node** ptr = &listTable[slot];
	while (*ptr != NULL) {
		if (Compare::equal((*ptr)->key, key)) {
			Node* t = (*ptr)->next;
			delete *ptr;
			*ptr = t;
			nodeCount--;
			if (nodeCount < (tableSize / 4) && tableSize >= 2 * kMinSize) {
				resize(Math::getNextPrime(tableSize / 2));
			}
			return true;
		}
		ptr = &((*ptr)->next);
	}
	return false;
}

HASH_MAP(void)::resize(size_t newSize) {
	Node** tmpListTable = new Node*[newSize];
	for (size_t i = 0; i < newSize; ++i) {
		tmpListTable[i] = NULL;
	}
	for (size_t i = 0; i < tableSize; ++i) {
		Node* node = listTable[i];
		while (node != NULL) {
			size_t slot = HashValue::hash(node->key) % newSize;
			Node* next = node->next;
			node->next = tmpListTable[slot];
			tmpListTable[slot] = node;
			node = next;
		}
	}
	delete[] listTable;
	listTable = tmpListTable;
	tableSize = newSize;
}

#undef HASH_MAP
#undef FULL_TMPL

} // namespace muddy

#endif // MUDDY_COMMON_ALGORITHM_HASH_TABLE_HPP_INCLUDED_
