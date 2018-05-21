#ifndef __ITERABLE_HPP__
#define __ITERABLE_HPP__

template <typename It, typename CIt, typename Elem>
class Iterable {
public: 
	virtual It begin() = 0;
	virtual It end() = 0;
	virtual CIt begin() const = 0;
	virtual CIt end() const = 0;
};

#endif