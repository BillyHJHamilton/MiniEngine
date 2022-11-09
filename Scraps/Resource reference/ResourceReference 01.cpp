#include "ResourceReference.h"

#if UNIT_TESTS
void TestReferences()
{
	ResourceReference ref1;
	assert(!ref1);

	{
		ResourceOwner owner1(new Resource);

		ref1 = owner1;
		ResourceReference ref2(owner1);
		ResourceReference ref3(ref1);

		assert(owner1.IsValid());
		assert(ref1.IsValid());
		assert(ref2);
		assert(ref1 == ref2);
		assert(owner1 == ref1);
		assert(owner1.Get() == ref2.Get());

		assert(owner1->x == 1);
		assert(ref1->x == 1);
		assert(ref2->x == 1);
	}

	assert(!ref1.IsValid());

	ResourceOwner owner2;
	assert(!owner2.IsValid());
	ResourceReference ref4(owner2);
	assert(!ref4.IsValid());
	assert(owner2== ref4);
}
#endif // UNIT_TESTS
