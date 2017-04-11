class matrix {
public:
		matrix(int i, int j);
		matrix(const matrix &m) ;
		
		int numRows () ;
		int numCols () ;

		float *access(const int i, const int j) const;
		friend std::ostream &operator << (std::ostream &os, matrix &m) ;

		static matrix readMatrix (std::string filename);

private:
		matrix ();
		int rows ;
		int cols ;

		float *data;

}