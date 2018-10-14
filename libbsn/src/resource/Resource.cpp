#include "bsn/resource/Resource.hpp"

#include <stdexcept>

namespace bsn {
    namespace resource {

        using namespace std;

        Resource::Resource() :
            name(),
            capacity(100),
            currentLevel(100),
            unit(1) {};

        Resource::Resource(std::string mName, double mCapacity, double mCurrentLevel, double mUnit) {
            if(mCapacity <= 0){
                throw std::invalid_argument ("Capacity should not be negative or null");
            }

            if(mCurrentLevel < 0 || mCurrentLevel > mCapacity ) {
                throw std::invalid_argument ("Current level should not be negative or null nor bigger than the capacity");
            }

            if(mUnit < 0  || mUnit > mCapacity) {
                throw std::invalid_argument ("The resolution should not be negative or null nor bigger than the capacity");
            }

            name = mName;
            capacity = mCapacity;
            currentLevel = mCurrentLevel;
            unit = mUnit;
        }

        Resource::~Resource(){};
        
        void Resource::consume(const int mult) {
            currentLevel-= unit*mult;

            if(currentLevel < 0) currentLevel = 0;
        }

        void Resource::generate(const int mult) {
            currentLevel += unit*mult;
            
            if(currentLevel >= capacity) currentLevel = capacity;
        }

        void Resource::setName (const string _name) {
            name = _name;
        }

        string Resource::getName() const {
            return name;
        }

        void Resource::setCapacity (const double _cap) {
            capacity = _cap;
        }

        double Resource::getCapacity() const{
            return capacity;
        }

        void Resource::setCurrentLevel(const double _cLevel) {
            currentLevel = _cLevel;
        }

        double Resource::getCurrentLevel() const{
            return currentLevel;
        }

        void Resource::setUnit(const double _unit){
            unit = _unit;
        }

        double Resource::getUnit() const{
            return unit; 
        }
    }
}