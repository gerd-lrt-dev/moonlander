/**
 * @file uibuilder.h
 * @brief Helper class for building standardized UI elements.
 *
 * The UIBuilder class provides factory functions to create buttons, titles,
 * and copyright labels with consistent styling and sizing.
 */

#ifndef UIBUILDER_H
#define UIBUILDER_H

#include <QLabel>
#include <QPushButton>

/**
 * @class UIBuilder
 * @brief Helper class for standardized UI element creation.
 *
 * Provides methods to create buttons, labels, and page titles with
 * predefined styles and sizes.
 */
class UIBuilder
{
public:
    /**
     * @brief Constructor for UIBuilder.
     *
     * Can be used to initialize global styles if needed.
     */
    UIBuilder();

    /**
     * @brief Creates a QPushButton with consistent styling.
     * @param buttonName Text displayed on the button.
     * @param parent Optional parent widget.
     * @return Pointer to the created QPushButton.
     */
    QPushButton* createButton(const QString &buttonName, QWidget *parent);

    /**
     * @brief Creates a QLabel displaying a copyright message.
     * @param copyRightStr Text for the copyright.
     * @param parent Optional parent widget.
     * @return Pointer to the created QLabel.
     */
    QLabel* createCopyright(const QString &copyRightStr, QWidget *parent);

    /**
     * @brief Creates a QLabel used as a page title.
     * @param titleStr Text for the title.
     * @param parent Optional parent widget.
     * @return Pointer to the created QLabel.
     */
    QLabel* createPageTitle(const QString &titleStr, QWidget *parent);

private:
    // ==========================================
    // Member Variables
    // ==========================================
    struct STDButtonLayout
    {
        static const QString styleSheet; ///< Style sheet for standard buttons
        static const int buttonwidth     = 300; ///< Standard button width
        static const int buttonHeight    = 50;  ///< Standard button height
    };
};

#endif // UIBUILDER_H
