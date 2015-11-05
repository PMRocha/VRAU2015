using UnityEngine;
using System.Collections;

public class GameState : MonoBehaviour {

    private GameObject Player1;
    private GameObject Player2;
    private GameObject Reload1;
    private GameObject Reload2;
    private GameObject Attack1;
    private GameObject Attack2;
    private GameObject Shield1;
    private GameObject Shield2;
    private MeshRenderer MeshPlayer1;
    private MeshRenderer MeshPlayer2;
    private MeshRenderer MeshReload1;
    private MeshRenderer MeshReload2;
    private MeshRenderer MeshAttack1;
    private MeshRenderer MeshAttack2;
    private MeshRenderer MeshShield1;
    private MeshRenderer MeshShield2;

    private Robot[] Robots;

    public enum State { Waiting2, Waiting1, Ready, Player1, Player2, Fight, End}
    public static State state;
    public string[] action;

    public void ResolveFight() {
        switch (action[0])
        {
            case "attack":
                if (action[1] != "shield") {
                    Robots[1].takeDamage();
                }
                break;
            case "reload":
                Robots[0].reload();
                break;
        }
        switch (action[1])
        {
            case "attack":
                if (action[0] != "shield")
                {
                    Robots[0].takeDamage();
                }
                break;
            case "reload":
                Robots[1].reload();
                break;
        }
        if (Robots[0].dead || Robots[1].dead)
            state = State.End;
    }

    // Use this for initialization
    void Start() {

        action = new string[2];

        Robots = new Robot[2];

        Robots[0] = new Robot(1);
        Robots[1] = new Robot(2);

        Player1 = GameObject.FindGameObjectWithTag("Player1");
        Player2 = GameObject.FindGameObjectWithTag("Player2");

        /*Reload1 = GameObject.FindGameObjectWithTag("R1");
        Reload2 = GameObject.FindGameObjectWithTag("R2");

        Attack1 = GameObject.FindGameObjectWithTag("A1");
        Attack2 = GameObject.FindGameObjectWithTag("A2");

        Shield1 = GameObject.FindGameObjectWithTag("S1");
        Shield2 = GameObject.FindGameObjectWithTag("S2");

        MeshPlayer1 = Player1.GetComponent<MeshRenderer>();
        MeshPlayer2 = Player2.GetComponent<MeshRenderer>();

        MeshReload1 = Reload1.GetComponent<MeshRenderer>();
        MeshReload2 = Reload2.GetComponent<MeshRenderer>();

        MeshAttack1 = Attack1.GetComponent<MeshRenderer>();
        MeshAttack2 = Attack2.GetComponent<MeshRenderer>();

        MeshShield1 = Shield1.GetComponent<MeshRenderer>();
        MeshShield2 = Shield2.GetComponent<MeshRenderer>();    */    
    }
	
	// Update is called once per frame

	void Update () {

       /* if (MeshPlayer1.isVisible && MeshPlayer2.isVisible)
        {
            state = State.Ready;
        }

        else if (MeshPlayer1.isVisible || MeshPlayer2.isVisible)
        {
            state = State.Waiting1;
        }
        else if (state == State.Ready && (MeshAttack1.isVisible || MeshReload1.isVisible || MeshShield1.isVisible))
        {
            state = State.Player1;

            if (MeshAttack1.isVisible)
            {
                action[0] = "attack";
            }
            else if (MeshReload1.isVisible)
            {
                action[0] = "reload";
            }
            else if (MeshShield1.isVisible)
            {
                action[0] = "shield";
            }


        }
        else if (state == State.Ready && (MeshAttack2.isVisible || MeshReload2.isVisible || MeshShield2.isVisible))
        {
            state = State.Player2;

            if (MeshAttack2.isVisible)
            {
                action[1] = "attack";
            }
            else if (MeshReload2.isVisible)
            {
                action[1] = "reload";
            }
            else if (MeshShield2.isVisible)
            {
                action[1] = "shield";
            }
        }
        else if (state == State.Player2 && (MeshAttack1.isVisible || MeshReload1.isVisible || MeshShield1.isVisible))
        {
            state = State.Fight;

            if (MeshAttack1.isVisible)
            {
                action[0] = "attack";
            }
            else if (MeshReload1.isVisible)
            {
                action[0] = "reload";
            }
            else if (MeshShield1.isVisible)
            {
                action[0] = "shield";
            }


        }
        else if (state == State.Player1 && (MeshAttack2.isVisible || MeshReload2.isVisible || MeshShield2.isVisible))
        {
            state = State.Fight;

            if (MeshAttack2.isVisible)
            {
                action[1] = "attack";
            }
            else if (MeshReload2.isVisible)
            {
                action[1] = "reload";
            }
            else if (MeshShield2.isVisible)
            {
                action[1] = "shield";
            }
        }
        else if (state == State.Fight)
        {
            ResolveFight();
        }
        else
        {
            state = State.Waiting2;
        }*/

   }
}

public class Robot
{
    public int id;
    public int health;
    public int bullets;
    public bool dead;

    public Robot(int id) {
        this.id = id;
        health = 5;
        bullets = 5;
        dead = false;
    }

    public void reload() {
        if (bullets < 3)
            bullets++;
    }

    public void takeDamage()
    {
        health--;
        if (health == 0)
            dead=true;
    }

    public int fire() {
        if (bullets > 0) {
            bullets--;
            return 1;
        }
        return 0; 
    }
}