using UnityEngine;
using System.Collections;

public class GameState : MonoBehaviour {

    private GameObject Player1;
    private GameObject Player2;
    private MeshRenderer Mesh1;
    private MeshRenderer Mesh2;

    public enum State { Waiting2, Waiting1, Ready, Player1, Player2, Fight, End}
    public static State state;

    // Use this for initialization
    void Start() {
        Player1 = GameObject.FindGameObjectWithTag("Player1");
        Player2 = GameObject.FindGameObjectWithTag("Player2");
        Mesh1 = Player1.GetComponent<MeshRenderer>();
        Mesh2 = Player2.GetComponent<MeshRenderer>();
    }
	
	// Update is called once per frame

	void Update () {

        if (Mesh1.isVisible && Mesh2.isVisible) {
            state = State.Ready;
        }
        
        else if (Mesh1.isVisible || Mesh2.isVisible) {
            state = State.Waiting1;
        }

        else
        {
            state = State.Waiting2;
        }

        Debug.Log(state);

   }
}
