#include "HermiteMatrixSystem.h"

#include <Eigen/Dense>
#include <stdexcept>

HermiteSystem HermiteMatrixSystem::initializeSystem(std::size_t n) {
  return {std::vector<std::vector<double>>(n, std::vector<double>(n, 0.0)),
          std::vector<double>(n, 0.0), std::vector<double>(n, 0.0)};
}

void HermiteMatrixSystem::applyLeftBoundaryCondition(
    const std::vector<sf::Vector2f> &points, HermiteSystem &system) {
  // Natural boundary condition in Hermite form: second derivative is zero at
  // the left endpoint.
  system.M[0][0] = 2.0;
  system.M[0][1] = 1.0;

  system.Bx[0] = 3.0 * static_cast<double>(points[1].x - points[0].x);
  system.By[0] = 3.0 * static_cast<double>(points[1].y - points[0].y);
}

void HermiteMatrixSystem::applyInternalEquations(
    const std::vector<sf::Vector2f> &points, HermiteSystem &system) {
  const std::size_t n = points.size();

  for (std::size_t row = 1; row < n - 1; ++row) {
    const std::size_t i = row - 1;

    system.M[row][i] = 1.0;
    system.M[row][i + 1] = 4.0;
    system.M[row][i + 2] = 1.0;

    system.Bx[row] = 3.0 * static_cast<double>(points[i + 2].x - points[i].x);
    system.By[row] = 3.0 * static_cast<double>(points[i + 2].y - points[i].y);
  }
}

void HermiteMatrixSystem::applyRightBoundaryCondition(
    const std::vector<sf::Vector2f> &points, HermiteSystem &system) {
  const std::size_t n = points.size();

  // Natural boundary condition in Hermite form: second derivative is zero at
  // the right endpoint.
  system.M[n - 1][n - 2] = 1.0;
  system.M[n - 1][n - 1] = 2.0;

  system.Bx[n - 1] = 3.0 * static_cast<double>(points[n - 1].x - points[n - 2].x);
  system.By[n - 1] = 3.0 * static_cast<double>(points[n - 1].y - points[n - 2].y);
}

HermiteSystem
HermiteMatrixSystem::buildSystem(const std::vector<sf::Vector2f> &points) {
  if (points.size() < 2) {
    throw std::invalid_argument("Need at least 2 control points.");
  }

  HermiteSystem system = initializeSystem(points.size());

  if (points.size() == 2) {
    system.M[0][0] = 2.0;
    system.M[0][1] = 1.0;
    system.M[1][0] = 1.0;
    system.M[1][1] = 2.0;

    const double dx = static_cast<double>(points[1].x - points[0].x);
    const double dy = static_cast<double>(points[1].y - points[0].y);

    system.Bx[0] = 3.0 * dx;
    system.By[0] = 3.0 * dy;
    system.Bx[1] = 3.0 * dx;
    system.By[1] = 3.0 * dy;

    return system;
  }

  applyLeftBoundaryCondition(points, system);
  applyInternalEquations(points, system);
  applyRightBoundaryCondition(points, system);

  return system;
}

std::vector<sf::Vector2f>
HermiteMatrixSystem::solveDerivatives(const std::vector<sf::Vector2f> &points) {
  if (points.size() < 2) {
    throw std::invalid_argument("Need at least 2 control points.");
  }

  const HermiteSystem system = buildSystem(points);
  const int n = static_cast<int>(points.size());

  Eigen::MatrixXd M(n, n);
  Eigen::VectorXd Bx(n);
  Eigen::VectorXd By(n);

  for (int row = 0; row < n; ++row) {
    Bx(row) = system.Bx[static_cast<std::size_t>(row)];
    By(row) = system.By[static_cast<std::size_t>(row)];

    for (int col = 0; col < n; ++col) {
      M(row, col) = system.M[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
    }
  }

  Eigen::FullPivLU<Eigen::MatrixXd> lu(M);
  if (!lu.isInvertible()) {
    throw std::runtime_error("Hermite system matrix is not invertible.");
  }

  const Eigen::VectorXd Rx = lu.solve(Bx);
  const Eigen::VectorXd Ry = lu.solve(By);

  std::vector<sf::Vector2f> derivatives;
  derivatives.reserve(points.size());

  for (int i = 0; i < n; ++i) {
    derivatives.emplace_back(static_cast<float>(Rx(i)),
                             static_cast<float>(Ry(i)));
  }

  return derivatives;
}
