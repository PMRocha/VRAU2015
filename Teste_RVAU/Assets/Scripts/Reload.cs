using UnityEngine;
using System.Collections;

public class Reload : MonoBehaviour {
    private static int numBullets1;
    private static int numBullets2;
    private static GameObject[] bullets1;
    private static GameObject[] bullets2;

    // Use this for initialization
    void Start () {
        numBullets1 = 0;
        numBullets2 = 0;
        bullets1 = GameObject.FindGameObjectsWithTag("Bullet1");
        bullets2 = GameObject.FindGameObjectsWithTag("Bullet2");

    }
	
	// Update is called once per frame
	void Update () {
        if (GetComponent<MeshRenderer>().isVisible)
        {
            if(name == "R1" && numBullets1 < 3)
            {
                bullets1[numBullets1].active = true;
                numBullets1++;
            }
            else if (name == "R2" && numBullets2 < 3)
            {
                bullets2[numBullets2].active = true;
                numBullets2++;
            }
        }
        else
        {
            for(int i = 0; i < bullets1.Length; ++i)
                bullets1[i].active = false;

            for (int i = 0; i < bullets2.Length; ++i)
                bullets2[i].active = false;
        }



	}
}
