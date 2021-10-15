#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QCheckBox>

/*!
* Enumeration describing the nature of a message processed by the logger.
*/
enum class MessageType
{
	INFO_LOG,
	WARNING_LOG,
	ERROR_LOG
};

/*!
* Message format required by logger. Message consists of a source, content and a type indicating the purpose of the content.
*/
struct Message
{
	/*!
	* Message constructor.
	*/
	Message(MessageType type, QString source, QString content)
	{
		this->type = type;
		this->source = source;
		this->content = content;
	}

	MessageType type; /*! Indicates content intention */
	QString source; /*! Entity that generates the message */
	QString content; /*! Message description */
};

/*!
* Endpoint for all messages generated by the various software components.
* Displays all messages using a \class QTextEdit component.
*/
class Logger : public QWidget
{
	Q_OBJECT
		
public:
	/*!
	* Class constructor.
	* @param [in] parent Parent widget.
	*/
	Logger(QWidget* parent = Q_NULLPTR);

	/*!
	* Add message to log.
	* \param [in] message Message to add to log.
	*/
	void log(const Message& message);

	/*!
	* Clear the message log.
	*/
	void clearLog();

signals:
	/*!
	* Signal generated when the logger's hide button is clicked.
	*/
	void hideRequested();

private:
	QHBoxLayout* baseLayout; /*! Layout for message display and supporting widgets */
	QVBoxLayout* controlLayout; /*! Layout for the message display controls */
	QTextEdit* display; /*! Display for the logged messages */
	QPushButton* hideLog; /*! Button to trigger the hideRequested signal */
	QPushButton* clear; /*! Clear the message log */
	QCheckBox* showInfo; /*! Toggle if info messages are included in the message log */
	QCheckBox* showWarnings; /*! Toggle if warnings are included in the message log */
	QCheckBox* showErrors; /*! Toggle if errors are included in the message log */
	QList<Message> messageLog; /* Log of all messages received since construction or last clear operation */

	/*!
	* update the message display based on the current logger state.
	*/
	void refreshDisplay();

	/*!
	* Slot for hide log button click event.
	*/
	void hideLogClicked();
};