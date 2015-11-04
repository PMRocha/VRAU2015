using UnityEngine;
using UnityEngine.UI;

public class Atack : MonoBehaviour {

    public Text HealthText;

	// Use this for initialization
	void Start () {
    }
	
	// Update is called once per frame
	void Update () {
	    if(GameState.state == GameState.State.Ready)
        {
            HealthText.text = "";
        }
	}
}
