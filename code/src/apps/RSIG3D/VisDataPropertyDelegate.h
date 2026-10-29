#include "propertydelegate.h"

class VisDataPropertyDelegate : public PropertyEditor::PropertyDelegate {
  Q_OBJECT
 public:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const /* override */ {
      // Call the base class sizeHint to get the default size
      QSize size = PropertyEditor::PropertyDelegate::sizeHint(option, index);
      
      // Increase the height to make the item taller
      size.setHeight(size.height() + 10); // Adds 10 pixels of extra height

      return size;
    }
};
