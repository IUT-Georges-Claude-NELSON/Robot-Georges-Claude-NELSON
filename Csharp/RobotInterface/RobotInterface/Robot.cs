using System;
using System.Collections.Generic;

public class Robot
{
	public string receivedText = "";
	public float distanceTelemetreDroit;
	public float distanceTelemetreCentre;
	public float distanceTelemetreGauche;
	public float positionXOdo;
	public float positionYOdo;
	public float angleRadianOdo;
	public float vitesseLineaireOdo;
	public float vitesseAngulaireOdo;
	public float timestamp;


	public Robot()
	{
	}

	public Queue<byte> byteListReceived = new Queue<byte>();

}
