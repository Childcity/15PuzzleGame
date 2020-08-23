#ifndef IRundomIMAGEPROVIDER_H
#define IRundomIMAGEPROVIDER_H

class QImage;


namespace Dal {
namespace Image {


class IRundomImageProvider {
public:
  virtual ~IRundomImageProvider();
  virtual QImage getRundomImage() const = 0;
};


}
}



#endif // IRundomIMAGEPROVIDER_H
