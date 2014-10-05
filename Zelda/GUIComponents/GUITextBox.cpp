#include "GUITextBox.hpp"
#include "../Common/Input/GameInputCommand.hpp"
#include "../TextConverter.hpp"
#include "../Common/Util/Assert.hpp"
#include "../Common/Message/MessageHandler.hpp"
#include "../World/Messages/MessageShowText.hpp"

const unsigned int TEXT_BOX_CHARACTERS_PER_LINE = 20;
const unsigned int TEXT_BOX_NUM_ROWS = 3;
const Ogre::Real TEXT_REVEAL_TIME = 0.1;
const Ogre::Real TEXT_CURSOR_BLINK_TIME = 0.5;

CGUITextBox::CGUITextBox(const std::string &id,
                          CEntity *pParentEntity,
                          CEGUI::Window *pParentWindow,
                          const CEGUI::String &unparsedText,
                          std::shared_ptr<CGUITextBox::SResult> result)
  : CGUIOverlay(id, pParentEntity, pParentWindow, pParentWindow->createChild("DefaultWindow", id + "_root")),
    mResult(result),
    mCurrentRowToWrite(0),
    mTimer(0),
    mNextCharacterCounter(0),
    mCurrentLineSize(0),
    mRowsWritten(0),
    mCursorShown(false),
    mDisplayedRows(0),
    mStatus(NONE) {

  pause(PAUSE_MAP_UPDATE | PAUSE_PLAYER_UPDATE);

  mTextWindow = m_pRoot->createChild("OgreTray/StaticText", "text_window");
  mCompleteText = unparsedText;

  CTextConverter::getSingleton().convert(mCompleteText);

  mTextToDisplay = mCompleteText;

  onResume();
}

CGUITextBox::~CGUITextBox() {
  unpause(PAUSE_ALL);
  CMessageHandler::getSingleton().addMessage(new CMessageShowText("", mResult, CMessageShowText::FINISHED));
}

void CGUITextBox::update(Ogre::Real tpf) {
  CGUIOverlay::update(tpf);


  mTimer -= tpf;
  if (mTimer <= 0) {
    if (mStatus == REVEAL) {
      mTimer = TEXT_REVEAL_TIME;
      showNextCharacter();
    }
    else if (mStatus == WAITING) {
      mTimer = TEXT_CURSOR_BLINK_TIME;
      stripCursor();
      mCursorShown = !mCursorShown;
      if (mCursorShown) {
        mTextWindow->setText(mTextWindow->getText() + "_");
      }
    }
  }
}

void CGUITextBox::stripCursor() {
  if (mCursorShown) {
    mTextWindow->setText(mTextWindow->getText().substr(0, mTextWindow->getText().size() - 1));
  }
}

void CGUITextBox::showNextCharacter() {
  if (mNextCharacterCounter >= mTextToDisplay.size()) {
    if (mStatus == REVEAL)
      mStatus = QUESTION;

    return;
  }

  ++mCurrentLineSize;

  if (lineFull()) {
    mCurrentLineSize = 0;
    ++mRowsWritten;
    ++mDisplayedRows;
    if (mRowsWritten >= TEXT_BOX_NUM_ROWS) {
      if (mStatus == REVEAL)
        mStatus = WAITING;
      return;
    }
    else {
      mTextWindow->setText(mTextWindow->getText() + "\n");
    }

    if (mDisplayedRows >= TEXT_BOX_NUM_ROWS) {
      popFrontLine();
    }
  }



  CEGUI::utf32 characterToAdd(mTextToDisplay[mNextCharacterCounter]);
  ++mNextCharacterCounter;

  if (characterToAdd == ' ') {
    mTextWindow->setText(mTextWindow->getText() + characterToAdd);
    showNextCharacter();
  }
  else {
    mTextWindow->setText(mTextWindow->getText() + characterToAdd);
  }
}

int CGUITextBox::nextWordLength() {
  int pos = mTextToDisplay.find(' ', mNextCharacterCounter);
  if (pos == CEGUI::String::npos) {
    return 0;
  }
  return pos - mNextCharacterCounter;
}

void CGUITextBox::popFrontLine() {
  CEGUI::String text(mTextWindow->getText());
  int pos = text.find("\n");
  if (pos == CEGUI::String::npos) {
    return;
  }
  --mDisplayedRows;
  mTextWindow->setText(text.substr(pos + 1));
}

bool CGUITextBox::lineFull() {
  return mCurrentLineSize + nextWordLength() > TEXT_BOX_CHARACTERS_PER_LINE;
}

void CGUITextBox::onResume() {
  if (mStatus == QUESTION) {
    deleteLater();
    mResult->mMutex.lock();
    mResult->mResult = RESULT_CONTINUE;
    mResult->mMutex.unlock();
    return;
  }

  mStatus = REVEAL;

  stripCursor();

  if (mRowsWritten > 0) {
    popFrontLine();
    mTextWindow->setText(mTextWindow->getText() + "\n");
  }
  mCurrentLineSize = 0;
  mRowsWritten = 0;
  mCursorShown = false;
}

void CGUITextBox::receiveInputCommand(const CGameInputCommand &cmd) {
  if (mStatus == WAITING || mStatus == QUESTION) {
    if (cmd.getType() == GIC_INTERACT || cmd.getType() == GIC_SWORD) {
      if (cmd.getState() == GIS_PRESSED) {
        onResume();
      }
    }
  }
}
