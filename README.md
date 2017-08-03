# enuSpaceEigen
enuSpace plugin for Eigen (graphical logic block)

enuSpaceEigen는 enuSpace에서 외부 external plugin 모듈 Eigen를 로직 블럭(block)형태로 플로우 프로그래밍을 제공합니다.

![Alt text](/images/enuSpaceEigen.png "enuSpace plugin (Eigen)")

enuSpaceEigen github : https://github.com/EXPNUNI/enuSpaceEigen

enuSpace blog : http://enuspace.tistory.com/

enuSpace gitbook (guide) : https://expnuni.gitbooks.io/enuspace/

Eigen의 C++ 코드. Eigen 3 documentation

<pre><code>
#include <iostream>
#include <Eigen/Dense>
using namespace Eigen;
int main()
{
  MatrixXd m(2,2);
  m(0,0) = 3;
  m(1,0) = 2.5;
  m(0,1) = -1;
  m(1,1) = m(1,0) + m(0,1);
  std::cout << "Here is the matrix m:\n" << m << std::endl;
  VectorXd v(2);
  v(0) = 4;
  v(1) = v(0) - 1;
  std::cout << "Here is the vector v:\n" << v << std::endl;
}
</code></pre>

enuSpaceEigen를 이용하면 로직블럭을 이용하여 아래 그림과 같이 블럭(block)으로 프로그래밍을 수행할 수 있습니다.
  
![Alt text](/images/enuSpaceEigen_runtime.png "enuSpace plugin (Eigen)")  
