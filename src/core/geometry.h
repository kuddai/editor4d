#include <vector>
#include <Eigen/Dense>


namespace editor4d {
    // Stores information about 4D geometry
    // in the form of vertices and triangles indices (tailered to opengl format)
    class Geometry {
    public:
        Geometry(const std::vector<float> &vertices, const std::vector<int> &indices);

        Geometry(std::vector<float> &&vertices, std::vector<int> &&indices);

        std::vector<float>& GetVertices();

        std::vector<int>& GetIndices();

        std::vector<float> Project(const Eigen::Matrix<float, 5, 5> &transform) const;

    private:
        std::vector<float> vertices_; 
        std::vector<int> indices_;

        void Validate() const;
    };
} // namespace editor4d

