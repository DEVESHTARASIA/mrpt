/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2019, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */

#include <gtest/gtest.h>
#include <mrpt/io/CMemoryStream.h>
#include <mrpt/serialization/CArchive.h>
#include <mrpt/serialization/CSerializable.h>

using namespace mrpt::serialization;

namespace MyNS
{
class Foo : public CSerializable
{
	DEFINE_SERIALIZABLE(Foo, MyNS)
   public:
	int16_t value;
};
}  // namespace MyNS

IMPLEMENTS_SERIALIZABLE(Foo, CSerializable, MyNS);

uint8_t MyNS::Foo::serializeGetVersion() const { return 0; }
void MyNS::Foo::serializeTo(CArchive& out) const { out << value; }
void MyNS::Foo::serializeFrom(CArchive& in, uint8_t serial_version)
{
	in >> value;
}

TEST(Serialization, CustomClassSerialize)
{
	mrpt::rtti::registerClass(CLASS_ID(MyNS::Foo));

	MyNS::Foo a;
	a.value = 123;

	mrpt::io::CMemoryStream buf;
	auto arch = mrpt::serialization::archiveFrom(buf);
	arch << a;

	buf.Seek(0);
	MyNS::Foo b;
	arch >> b;

	EXPECT_EQ(a.value, b.value);
}

TEST(Serialization, ArchiveSharedPtrs)
{
	mrpt::io::CMemoryStream buf;
	auto arch_ptr = mrpt::serialization::archivePtrFrom(buf);
	auto arch_ptr2 = mrpt::serialization::archiveUniquePtrFrom(buf);

	int a = 42;
	(*arch_ptr) << a;
	buf.Seek(0);

	int b;
	(*arch_ptr2) >> b;

	EXPECT_EQ(a, b);
}
