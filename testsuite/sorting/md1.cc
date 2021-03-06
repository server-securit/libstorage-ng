
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE libstorage

#include <boost/test/unit_test.hpp>

#include "storage/Devicegraph.h"
#include "storage/Storage.h"
#include "storage/Environment.h"
#include "storage/Devices/Md.h"


using namespace std;
using namespace storage;


namespace std
{
    ostream& operator<<(ostream& s, const vector<Md*>& mds)
    {
	s << "{";
	for (vector<Md*>::const_iterator it = mds.begin(); it != mds.end(); ++it)
	    s << (it == mds.begin() ? " " : ", ") << (*it)->get_displayname();
	s << " }";

	return s;
    }
}


BOOST_AUTO_TEST_CASE(md_sorting1)
{
    Environment environment(true, ProbeMode::NONE, TargetMode::DIRECT);

    Storage storage(environment);

    Devicegraph* staging = storage.get_staging();

    Md* md10 = Md::create(staging, "/dev/md10");
    Md* md0 = Md::create(staging, "/dev/md0");
    Md* md2 = Md::create(staging, "/dev/md2");
    Md* md1 = Md::create(staging, "/dev/md1");

    Md* md_foo = Md::create(staging, "/dev/md/foo");
    Md* md_bar = Md::create(staging, "/dev/md/bar");

    Md* md_3 = Md::create(staging, "/dev/md/3");

    vector<Md*> all = Md::get_all(staging);
    sort(all.begin(), all.end(), Md::compare_by_name);

    BOOST_CHECK_EQUAL(all, vector<Md*>({
	md_3, md_bar, md_foo, md0, md1, md2, md10
    }));
}
