#ifndef RECOGNIZER_H_
#define RECOGNIZER_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include "global.hpp"

class Recognizer {
public:
	Recognizer();
	virtual ~Recognizer();
	/**
	 * @brief Herkent figuren van de gegeven vorm en kleur in een afbeelding.
	 *
	 * Het herkende figuur wordt omrand en er wordt een label geplaatst met extra informatie.
	 *
	 * @p original De originele afbeelding waarin het figuur herkent moet worden.
	 * @p shape De vorm die herkent moet worden.
	 * @p color De kleur die herkent moet worden.
	 *
	 * @return Een afbeelding waarin het figuur met de opgegeven specificaties omrand en gelabelt is.
	 */
	cv::Mat recognize(cv::Mat original, global::Shape shape,
			global::Color color);
private:
	/**
	 * @brief Filtert de opgegeven afbeelding zodat alleen de opgegeven kleur overblijft.
	 *
	 * De meegegeven afbeeldign wordt omgezet naar HSV formaat. Hierna worden kleuren mbv inRange weggefilterd zodat alleen de opgegeven kleur overblijft.
	 * De range kan owrden ingesteld met sliders.
	 *
	 * @p original De originele afbeelding waarin de kleur herkent moet worden.
	 * @p color De kleur die herkent moet worden.
	 *
	 * @return Gefilterde afbeelding waarin enkel de opgegeven kleur is weergegeven.
	 */
	cv::Mat findColor(cv::Mat original, global::Color color);

	/**
	 * @brief Berekent de afstand tussen twee punten en geef tdeze terug.
	 *
	 * @p point1  het eerste punt waar tussen de afstand wordt berekend.
	 * @p point2 het tweede punt waar tussen de afstand wordt berekend.
	 *
	 *
	 * @return Afstand
	 */

	float calculateDistance(cv::Point point1, cv::Point point2);

	/**
	 * @brief Controleert of een vorm een vierkant is.
	 *
	 * Vergelijkt de lengte met de breedte, hier mag niet meer dan 10% afwijking inzitten.
	 *
	 * @p approximation Een vector met alle hoekpunten
	 *
	 * @return true wanneer vierkent anders false
	 */

	bool checkSquare(std::vector<cv::Point> approximation);
	/**
	 * @brief Controleert of een vorm een halve cirkel is.
	 *
	 * Er wordt een zo'n klein mogelijke rechthoek om de vorm heen getekend.
	 * Het oppervlak van het figuur wordt gedeeld door het oppervlak van deze rechthoek.
	 * Wanneer het getal na de deling binnen een bepaalde marge zit is de vorm een cirkel of halve cirkel.
	 * Wanneer de rechthoek een vierkant is, is de vorm een cirkel. Op basis van deze feiten wordt bepaald of de vorm een halve cirkel is,
	 *
	 *
	 * @p approximation een vector emt alle hoekpunten
	 *
	 *
	 * @return true wanneer halve cirkel anders  false.
	 */

	bool checkHalfCircle(std::vector<cv::Point> approximation);
	/**
	 * @brief Detecteert de opgegeven vorm in een afbeelding.
	 *
	 * @p original De originele afbeelding waarin de vorm herkent moet worden.
	 * @p shape De vorm die herkend moet worden.
	 *
	 * @return Een vectore met daarin de contours
	 */
	std::vector<std::vector<cv::Point>> findShape(cv::Mat filtered,
			global::Shape shape);

	/*
	 * @brief Tekent een omlijning op de meegegeven afbeelding om de meegegeven contouren en plaatst een label.
	 *
	 * Wanneer er een figuur is gevonden wordt er een omlijning om dit figuur getekent. Het middenpunt van het figuur wordt bepaalt en hier wordt een label geplaatst.
	 *
	 * @p original De originele afbeelding waarop getekend moet worden.
	 * @p contours De hoekpunten van de vorm die omlijnt moet worden.
	 * @p duration De tijd die het duurt om een figuur te herkennen.
	 *
	 * @return De originele afbeelding waarop de contouren zijn getekend en een label is geplaatst.
	 *
	 */
	cv::Mat draw(cv::Mat original,
			std::vector<std::vector<cv::Point>> const &contours,
			float duration);
	/**
	 * @brief Plaatst een label met tekst op een afbeelding.
	 *
	 *  Dit label wordt in het midden van een figuur geplaatst en bevat de oppervlakte van dit figuur. Verder bevat het label ook de tijd die het gekost heeft om het figuur te herkennen.
	 *
	 * @p original De originele afbeelding waarop het label wordt geplaatst.
	 * @p contours De hoekpunten van het figuur waarin het label wordt geplaatst.
	 * @p duration De tijd die het duurt om een figuur te herkennen.
	 *
	 * @return De originele afbeelding waarop het label is geplaatst.
	 */

	cv::Mat text(cv::Mat original, cv::Point const &center, double area,
			float duration);
};

#endif /* RECOGNIZER_H_ */
