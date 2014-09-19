#include "GUITextBox.hpp"
#include "../Common/Input/GameInputCommand.hpp"

const unsigned int TEXT_BOX_CHARACTERS_PER_LINE = 20;
const unsigned int TEXT_BOX_NUM_ROWS = 3;
const Ogre::Real TEXT_REVEAL_TIME = 0.1;
const Ogre::Real TEXT_CURSOR_BLINK_TIME = 0.5;

CGUITextBox::CGUITextBox(const std::string &id,
                          CEntity *pParentEntity,
                          CEGUI::Window *pParentWindow)
  : CGUIOverlay(id, pParentEntity, pParentWindow, pParentWindow->createChild("DefaultWindow", id + "_root")),
    mCurrentRowToWrite(0),
    mTimer(0),
    mNextCharacterCounter(0),
    mCurrentLineSize(0),
    mRowsWritten(0),
    mCursorShown(false),
    mDisplayedRows(0) {

  pause(PAUSE_ALL_WORLD_UPDATE);

  mTextWindow = m_pRoot->createChild("OgreTray/StaticText", "text_window");
  mCompleteText = "asdfa asdfasdf asdf asdfasdfa asdf asdf asd fasdfkjsdlf asldkal dsajfkjsf ajdf askfja asf";
  mTextToDisplay = mCompleteText;

  onResume();
}

CGUITextBox::~CGUITextBox() {
  unpause(PAUSE_ALL);
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
    else if (mStatus == QUESTION) {
      deleteLater();
    }
  }
}

void CGUITextBox::stripCursor() {
  if (mCursorShown) {
    mTextWindow->setText(mTextWindow->getText().substr(0, mTextWindow->getText().size() - 1));
  }
}

void CGUITextBox::showNextCharacter() {
  if (mNextCharacterCounter >= mCompleteText.size()) {
    mStatus = QUESTION;
    return;
  }

  ++mCurrentLineSize;

  if (lineFull()) {
    mCurrentLineSize = 0;
    ++mRowsWritten;
    ++mDisplayedRows;
    if (mRowsWritten >= TEXT_BOX_NUM_ROWS) {
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



  CEGUI::utf32 characterToAdd(mCompleteText[mNextCharacterCounter]);
  ++mNextCharacterCounter;
  mTextWindow->setText(mTextWindow->getText() + characterToAdd);

  if (characterToAdd == ' ') {
    showNextCharacter();
  }
}

int CGUITextBox::nextWordLength() {
  int pos = mCompleteText.find(' ', mNextCharacterCounter);
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
  stripCursor();

  if (mRowsWritten > 0) {
    popFrontLine();
    mTextWindow->setText(mTextWindow->getText() + "\n");
  }
  mRowsWritten = 0;
  mStatus = REVEAL;
  mCursorShown = false;
}

void CGUITextBox::receiveInputCommand(const CGameInputCommand &cmd) {
  if (mStatus == WAITING) {
    if (cmd.getType() == GIC_INTERACT || cmd.getType() == GIC_SWORD) {
      if (cmd.getState() == GIS_PRESSED) {
        onResume();
      }
    }
  }
}
