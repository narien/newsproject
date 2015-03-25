//Database on memeory

#ifndef DATABASEMEM_H
#define DATABASEMEM_H

#include <map>
#include <string>

using namespace std;

//potentially move later
struct Article {

		string title;
		string author;
		string text;
};

struct Newsgroup {

		int article_cntr = 1;
		string title;
		map<int, Article> articles;
};

class DatabaseMem {
	public:
		DatabaseMem();
		~DatabaseMem();
		bool insert_newsgroup(string title);
		bool insert_article(string& newsgroup_name, string& article_title, string& article_author, string& article_text);
		bool remove(string name);

	private:
		int newsgrp_cntr; //unique id number
		map<int, Newsgroup> storage;
};

#endif