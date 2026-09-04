/**
 * @file coordinatehelppage.h
 * @brief Coordinate frame reference page for the simulator frontend.
 *
 * Presents concise descriptions and schematic visualizations of the
 * coordinate systems currently used by SDF.
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
     * @brief Creates one frame-reference card with a compact schematic.
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

    /**
     * @brief Creates the schematic visualization for a coordinate frame.
     *
     * @param frameName Short frame identifier used to select the diagram.
     * @return Widget containing the schematic frame visualization.
     */
    QWidget *createFrameDiagram(const QString& frameName);
};

#endif // COORDINATEHELPPAGE_H
