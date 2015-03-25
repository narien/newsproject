//Database in memory

#include "databasemem.h"


DatabaseMem::DatabaseMem() {

	newsgrp_cntr = 1;
}

bool DatabaseMem::insert_newsgroup(string& title) {

	if (title.empty()) {

		Newsgroup tmp_grp = Newsgroup();
		tmp_grp.title = title;
		storage.insert({newsgrp_cntr, tmp_grp});

		newsgrp_cntr++;

		return true;
	}

	else {
		return false;
	}
}

bool DatabaseMem::insert_article(int& newsgroup_id, string& article_title, string& article_author, string& article_text) {

	return false;
}


bool DatabaseMem::remove_newsgroup(int& newsgroup_id) {

/*		if (title != NULL) {

		storage.remove();


		return true;
	}

	else {
		return false;
	}*/
}

bool DatabaseMem::remove_article(int& newsgroup_id, int& article_id) {

}


DatabaseMem::~DatabaseMem() {

}

//tmp test
int main() {

}