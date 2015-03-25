//Database in memory

#include "databasememory.h"


DatabaseMemory::DatabaseMemory() {

	newsgrp_cntr = 1;
}

bool DatabaseMemory::insert_newsgroup(string& title) {

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

bool DatabaseMemory::insert_article(int& newsgroup_id, string& article_title, string& article_author, string& article_text) {

	return false;
}


bool DatabaseMemory::remove_newsgroup(int& newsgroup_id) {

	return false;
}

bool DatabaseMemory::remove_article(int& newsgroup_id, int& article_id) {

	return false;
}


DatabaseMemory::~DatabaseMemory() {

}

//tmp test
int main() {

}