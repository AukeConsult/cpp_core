// collection interfaces


template <typename Type>
class Collection {

public:
	virtuel Type* 	get(size_t pos)() {};
	virtuel void 	put(Type* element)() {};	
protected:
	size_type max_size;
}