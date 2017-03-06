#!/bin/bash

echo "Running create_database.sh"
sqlite3 mo.db "create table tables(name text not null);"
sqlite3 mo.db "create table mo_types(type int, name text not null);"
sqlite3 mo.db "insert into mo_types(type,name) values (2003828737, 'type_kotek');"
sqlite3 mo.db "insert into mo_types(type,name) values (2003828738, 'type_owca');"
sqlite3 mo.db "insert into mo_types(type,name) values (2003828739, 'type_krolik');"
sqlite3 mo.db "create table mo_list(id int primary key not null, type text not null, name text not null);"
sqlite3 mo.db "create table type_kotek(id int primary key not null, name text not null, attr1 text, attr2 text);"
sqlite3 mo.db "create table type_owca(id int primary key not null, name text not null, attr1 text, attr2 text);"
sqlite3 mo.db "create table type_krolik(id int primary key not null, name text not null, attr1 text, attr2 text);"
sqlite3 mo.db "insert into tables(name) values ('mo_types'), ('mo_list'), ('type_kotek'), ('type_owca'), ('type_krolik');"
