#include "test.h"

#include "reader_nsf.h"

int main()
{
	//invindex* ii;
	//ii = create_invindex_entry_alist();
	//add_tf_to_ii(ii, 12319, 350, 3);

	idmap* m = create_idmap_entry_alist();
	nsf_load_bow_idnsfid(m, "../dataset/nsf/bow/1/idnsfid.txt");
}
