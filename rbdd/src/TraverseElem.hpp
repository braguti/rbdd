
#include <Rcpp.h>

class TraverseElem {

	private:
		int position;
		int index;
		int high;
		int low;
		bool mark;
		Rcpp::NumericVector result;

	public:
		TraverseElem();
		int getPosition();
		void setPosition(int position);
		int getIndex();
		void setIndex(int index);
		int getHigh();
		void setHigh(int high);
		int getLow();
		void setLow(int low);
		bool getMark();
		void setMark(bool mark);
		Rcpp::NumericVector getResult();
		void setResult(Rcpp::NumericVector result);
};
