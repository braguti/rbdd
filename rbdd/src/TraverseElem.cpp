
#include "TraverseElem.hpp"

TraverseElem::TraverseElem() {
}

int TraverseElem::getPosition() {
	return this->position;
}

void TraverseElem::setPosition(int position) {
	this->position = position;
}

int TraverseElem::getIndex() {
	return this->index;
}

void TraverseElem::setIndex(int index) {
	this->index = index;
}

int TraverseElem::getHigh() {
	return this->high;
}

void TraverseElem::setHigh(int high) {
	this->high = high;
}

int TraverseElem::getLow() {
	return this->low;
}

void TraverseElem::setLow(int low) {
	this->low = low;
}

bool TraverseElem::getMark() {
	return this->mark;
}

void TraverseElem::setMark(bool mark) {
	this->mark = mark;
}

Rcpp::NumericVector TraverseElem::getResult() {
	return this->result;
}

void TraverseElem::setResult(Rcpp::NumericVector result) {
	this->result = result;
}
