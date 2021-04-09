using System;
using System.Collections.Generic;

public class Robot
{
	public string receivedText = "";
	public float distanceTelemetreDroit;
	public float distanceTelemetreCentre;
	public float distanceTelemetreGauche;

	public Robot()
	{
	}

	public Queue<byte> byteListReceived = new Queue<byte>();

}
