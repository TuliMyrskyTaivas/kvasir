//////////////////////////////////////////////////////////////////////////
/// file: group.h
///
/// summary: scan group
//////////////////////////////////////////////////////////////////////////

#ifndef KVASIR_GROUP_H_INCLUDED
#define KVASIR_GROUP_H_INCLUDED

namespace kvasir
{

template<typename Type> class SystemImpl;

template<typename Type>
class Group
{
	friend class SystemImpl<Type>;
	Group();

public:
	~Group() = default;
};

} // namespace kvasir

#endif // KVASIR_GROUP_H_INCLUDED