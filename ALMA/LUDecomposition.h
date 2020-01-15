#pragma once
#include <algorithm>
#include"Matrix.h"
template <typename Matrix>
class LUDecomposition
{
	/** Array for internal storage of decomposition.
	@serial internal array storage.
	*/
private:
	std::vector< std::vector<double>> LU ;

	/** Row and column dimensions, and pivot sign.
	@serial column dimension.
	@serial row dimension.
	@serial pivot sign.
	*/
	int m, n, pivsign;

	/** Internal storage of pivot vector.
	@serial pivot vector.
	*/
	 std::vector<int> piv;
public:
	
	 LUDecomposition(Matrix* const A) {

		// Use a "left-looking", dot-product, Crout/Doolittle algorithm.

		LU = A->getArrayCopy();
		m = A->getRowDimension();
		n = A->getColumnDimension();
		piv.resize(m);
		for (int i = 0; i < m; i++) {
			piv[i] = i;
		}
		pivsign = 1;
		// std::vector<double> LUrowi;
		std::vector<double> LUcolj(m);

		// Outer loop.

		for (int j = 0; j < n; j++) {

			// Make a copy of the j-th column to localize references.

			for (int i = 0; i < m; i++) {
				LUcolj[i] = LU[i][j];
			}

			// Apply previous transformations.

			for (int i = 0; i < m; i++) {
				auto &LUrowi = LU[i];

				// Most of the time is spent in the following dot product.

				int kmax = std::min(i, j);
				double s = 0.0;
				for (int k = 0; k < kmax; k++) {
					s += LUrowi[k] * LUcolj[k];
				}

				LUrowi[j] = LUcolj[i] -= s;
			}

			// Find pivot and exchange if necessary.

			int p = j;
			for (int i = j + 1; i < m; i++) {
				if (abs(LUcolj[i]) > abs(LUcolj[p])) {
					p = i;
				}
			}
			if (p != j) {
				for (int k = 0; k < n; k++) {
					double t = LU[p][k]; LU[p][k] = LU[j][k]; LU[j][k] = t;
				}
				int k = piv[p]; piv[p] = piv[j]; piv[j] = k;
				pivsign = -pivsign;
			}

			// Compute multipliers.

			if (j < m & LU[j][j] != 0.0) {
				for (int i = j + 1; i < m; i++) {
					LU[i][j] /= LU[j][j];
				}
			}
		}
	}

	  bool isNonsingular() {
		 for (int j = 0; j < n; j++) {
			 if (LU[j][j] == 0)
				 return false;
		 }
		 return true;
	 }

	 /** Return lower triangular factor
	 @return     L
	 */

	  Matrix getL() {
		 Matrix X =  Matrix(m, n);
		 std::vector< std::vector<double>> L = X.getArray();
		 for (int i = 0; i < m; i++) {
			 for (int j = 0; j < n; j++) {
				 if (i > j) {
					 L[i][j] = LU[i][j];
				 }
				 else if (i == j) {
					 L[i][j] = 1.0;
				 }
				 else {
					 L[i][j] = 0.0;
				 }
			 }
		 }
		 return X;
	 }

	 /** Return upper triangular factor
	 @return     U
	 */

	  Matrix getU() {
		 Matrix X =  Matrix(n, n);
		 std::vector< std::vector<double>> U = X.getArray();
		 for (int i = 0; i < n; i++) {
			 for (int j = 0; j < n; j++) {
				 if (i <= j) {
					 U[i][j] = LU[i][j];
				 }
				 else {
					 U[i][j] = 0.0;
				 }
			 }
		 }
		 return X;
	 }

	 /** Return pivot permutation vector
	 @return     piv
	 */

	  std::vector<int> getPivot() {
		 std::vector<int> p ;
		 for (int i = 0; i < m; i++) {
			 p[i] = piv[i];
		 }
		 return p;
	 }

	 /** Return pivot permutation vector as a one-dimensional double array
	 @return     (double) piv
	 */

	  std::vector<double> getDoublePivot() {
		 std::vector<double> vals;
		 for (int i = 0; i < m; i++) {
			 vals[i] = (double)piv[i];
		 }
		 return vals;
	 }

	 /** Determinant
	 @return     det(A)
	 @exception  IllegalArgumentException  Matrix must be square
	 */

	  double det() {
		 if (m != n) {
			 throw std::invalid_argument("Matrix must be square.");
		 }
		 double d = (double)pivsign;
		 for (int j = 0; j < n; j++) {
			 d *= LU[j][j];
		 }
		 return d;
	 }

	 /** Solve A*X = B
	 @param  B   A Matrix with as many rows as A and any number of columns.
	 @return     X so that L*U*X = B(piv,:)
	 @exception  IllegalArgumentException Matrix row dimensions must agree.
	 @exception  RuntimeException  Matrix is singular.
	 */

	  Matrix solve(Matrix B) {
		 if (B.getRowDimension() != m) {
			 throw std::invalid_argument("Matrix row dimensions must agree.");
		 }
		 if (!this->isNonsingular()) {
			 throw std::invalid_argument("Matrix is singular.");
		 }

		 // Copy right hand side with pivoting
		 int nx = B.getColumnDimension();
		 Matrix Xmat = B.getMatrix(piv, 0, nx - 1);
		 std::vector< std::vector<double>> X = Xmat.getArray();

		 // Solve L*Y = B(piv,:)
		 for (int k = 0; k < n; k++) {
			 for (int i = k + 1; i < n; i++) {
				 for (int j = 0; j < nx; j++) {
					 X[i][j] -= X[k][j] * LU[i][k];
				 }
			 }
		 }
		 // Solve U*X = Y;
		 for (int k = n - 1; k >= 0; k--) {
			 for (int j = 0; j < nx; j++) {
				 X[k][j] /= LU[k][k];
			 }
			 for (int i = 0; i < k; i++) {
				 for (int j = 0; j < nx; j++) {
					 X[i][j] -= X[k][j] * LU[i][k];
				 }
			 }
		 }
		 return X;
	 }
};
