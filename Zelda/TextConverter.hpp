#ifndef _TEXT_CONVERTER_HPP_
#define _TEXT_CONVERTER_HPP_

#include <OgreSingleton.h>
#include "Common/Message/MessageInjector.hpp"
#include <CEGUI/String.h>

class CMap;

class CTextConverter : public Ogre::Singleton<CTextConverter>, public CMessageInjector {
protected:
  const CMap *mCurrentMap;
public:
  CTextConverter();
  virtual ~CTextConverter();

  static CTextConverter &getSingleton();
  static CTextConverter *getSingletonPtr();

  void convert(CEGUI::String &text);

private:
  virtual void sendMessageToAll(const CMessage &);
};

#endif // _TEXT_CONVERTER_HPP_
