/**
 * @file coordinatehelppage.h
 * @brief Coordinate frame reference page for the simulator frontend.
 *
 * Presents the authoritative SDF coordinate-frame overview together with
 * concise descriptions of the currently supported reference frames.
 */

#ifndef COORDINATEHELPPAGE_H
#define COORDINATEHELPPAGE_H

#include <QString>
#include <QWidget>

class QFrame;

/**
 * @class CoordinateHelpPage
 * @brief User-facing reference for SDF coordinate systems.
 *
 * The page documents the purpose, representation and relationships of the
 * coordinate systems currently used by the CoordinateTransformer:
 * MCI, MCMF, MSC, ENU, LVLH and SBF.
 *
 * The page is intentionally frontend-only and contains no transformation
 * logic. Coordinate mathematics remains exclusively in the backend.
 */
class CoordinateHelpPage : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the coordinate frame reference page.
     * @param parent Optional parent widget.
     */
    explicit CoordinateHelpPage(QWidget *parent = nullptr);

    /**
     * @brief Default destructor.
     */
    ~CoordinateHelpPage() = default;

private:

    /**
     * @brief Builds the complete coordinate reference page.
     */
    void setupUI();

    /**
     * @brief Creates one textual frame-reference card.
     *
     * @param frameName Short frame identifier.
     * @param title Human-readable frame name.
     * @param description Purpose and frame semantics.
     * @param coordinates Coordinate representation / axis convention.
     * @param origin Frame origin or reference location.
     * @return Newly created frame widget.
     */
    QFrame *createFrameCard(
        const QString& frameName,
        const QString& title,
        const QString& description,
        const QString& coordinates,
        const QString& origin);
};

#endif // COORDINATEHELPPAGE_H
