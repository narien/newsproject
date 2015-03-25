//Database in memory

#include "databasemem.h"


DatabaseMem::DatabaseMem() {

	newsgrp_cntr = 1;
}

bool DatabaseMem::insert_newsgroup(string title) {

	Newsgroup tmp_grp = Newsgroup();

	tmp_grp.title = title;

	storage.insert({newsgrp_cntr, tmp_grp});

	newsgrp_cntr++;

	return false;

}

bool DatabaseMem::insert_article(string& newsgroup_name, string& article_title, string& article_author, string& article_text) {

	return false;
}

bool DatabaseMem::remove(string name) {

	return false;
}


DatabaseMem::~DatabaseMem() {

}

//tmp test
int main() {

}