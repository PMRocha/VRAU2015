using UnityEngine;
using System.Collections;

public class ButtonBehaviour : MonoBehaviour {

    public GameObject loadingScreen;

	// Use this for initialization
	void Start () {
        loadingScreen.SetActive(false);
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    public void Quit()
    {
        Application.Quit();
    }

    public void ChangeScene(int scene)
    {
        loadingScreen.SetActive(true);
        Application.LoadLevel(scene);
    }
}
