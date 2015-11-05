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
                if (Robots[1].fire() == 1)
                {
                    if (action[1] != "shield")
                    {
                        Robots[1].takeDamage();
                    }
                }
                break;
            case "reload":
                Robots[0].reload();
                break;
        }
        switch (action[1])
        {
            case "attack":
                if (Robots[1].fire()==1) { 
                    if (action[0] != "shield")
                    {
                        Robots[0].takeDamage();
                    }
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

        Reload1 = GameObject.FindGameObjectWithTag("R1");
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
        MeshShield2 = Shield2.GetComponent<MeshRenderer>();     
    }
	
	// Update is called once per frame

	void Update () {

        if (MeshPlayer1.isVisible && MeshPlayer2.isVisible)
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
        }

   }
}

public class Robot
{
    public int id;
    public int health;
    public int bullets;
    public bool dead;

    private GameObject[] Bullets;
    private GameObject[] HealthBar;

    private MeshRenderer[] BulletsMeshes;

    public Robot(int id) {
        this.id = id;
        health = 5;
        bullets = 5;
        dead = false;

        Bullets = new GameObject[3];
        BulletsMeshes = new MeshRenderer[3];
        HealthBar = new GameObject[5];

        if (id == 1)
        {
            Bullets[0] = GameObject.FindGameObjectWithTag("Bullet11");
            Bullets[1] = GameObject.FindGameObjectWithTag("Bullet12");
            Bullets[2] = GameObject.FindGameObjectWithTag("Bullet13");

            BulletsMeshes[0] = Bullets[0].GetComponent<MeshRenderer>();
            BulletsMeshes[1] = Bullets[1].GetComponent<MeshRenderer>();
            BulletsMeshes[2] = Bullets[2].GetComponent<MeshRenderer>();

            HealthBar[0] = GameObject.FindGameObjectWithTag("Health11");
            HealthBar[1] = GameObject.FindGameObjectWithTag("Health12");
            HealthBar[2] = GameObject.FindGameObjectWithTag("Health13");
            HealthBar[3] = GameObject.FindGameObjectWithTag("Health14");
            HealthBar[4] = GameObject.FindGameObjectWithTag("Health15");
        }
        else
        {
            Bullets[0] = GameObject.FindGameObjectWithTag("Bullet21");
            Bullets[1] = GameObject.FindGameObjectWithTag("Bullet22");
            Bullets[2] = GameObject.FindGameObjectWithTag("Bullet23");

            BulletsMeshes[0] = Bullets[0].GetComponent<MeshRenderer>();
            BulletsMeshes[1] = Bullets[1].GetComponent<MeshRenderer>();
            BulletsMeshes[2] = Bullets[2].GetComponent<MeshRenderer>();

            HealthBar[0] = GameObject.FindGameObjectWithTag("Health21");
            HealthBar[1] = GameObject.FindGameObjectWithTag("Health22");
            HealthBar[2] = GameObject.FindGameObjectWithTag("Health23");
            HealthBar[3] = GameObject.FindGameObjectWithTag("Health24");
            HealthBar[4] = GameObject.FindGameObjectWithTag("Health25");
        }

        Bullets[0].SetActive(false);
        Bullets[1].SetActive(false);
        Bullets[2].SetActive(false);
    }

    public void reload() {
        if (bullets < 3)
        {
            bullets++;
            Bullets[bullets - 1].SetActive(true);
        }
    }

    public void takeDamage()
    {
        health--;
        HealthBar[health].SetActive(false);
        if (health == 0)
            dead=true;
    }

    public int fire() {
        if (bullets > 0) {
            bullets--;
            Bullets[bullets].SetActive(false);
            return 1;
        }
        return 0; 
    }
}