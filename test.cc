//
// Created by Artur Kaleta on 23/08/2019.
//
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "Database.h"
#include "Image.h"

Database db;

TEST_CASE("database file parsing", "[DB]") {
    SECTION("parsing test file") {
        REQUIRE(db.parseFile("data/1.txt"));
    }SECTION("checking number of records") {
        REQUIRE(db.size() == 2);
    }
}

TEST_CASE("database searching", "[SEARCH]") {
    auto images = db.findImage("1234");
    SECTION("checking returned items count") {
        REQUIRE(images.size() == 2);
    }SECTION("checking SWM match with original") {
        REQUIRE(images[0].getSwm() == "20061234");
    }SECTION("checking if wrong SWM doesn't return data") {
        REQUIRE(db.findImage("9999").empty());
    }
}





