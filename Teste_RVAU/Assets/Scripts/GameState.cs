using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class GameState : MonoBehaviour {

    private float speed = 250;
    private float waitingTime = 2.5f;

    private GameObject Player1;
    private GameObject Player2;
    private GameObject Reload1;
    private GameObject Reload2;
    private GameObject Attack1;
    private GameObject Attack2;
    private GameObject Shield1;
    private GameObject Shield2;
    private GameObject ShieldObject1;
    private GameObject ShieldObject2;
    private GameObject ReloadObject1;
    private GameObject ReloadObject2;
    private GameObject BulletObject1;
    private GameObject BulletObject2;

    private MeshRenderer MeshPlayer1;
    private MeshRenderer MeshPlayer2;
    private MeshRenderer MeshReload1;
    private MeshRenderer MeshReload2;
    private MeshRenderer MeshAttack1;
    private MeshRenderer MeshAttack2;
    private MeshRenderer MeshShield1;
    private MeshRenderer MeshShield2;

    private Robot robot1;
    private Robot robot2;

    private float firstTime;

    public enum State { Waiting2, Waiting1, Ready, Player1, Player2, Fight, Wait, End}
    public static State state;
    public string[] action;
    public Text StateText;

    public void ResolveFight() {
        switch (action[0])
        {
            case "attack":
                if (robot1.fire() )
                {
                    BulletObject1.SetActive(true);
                    if (action[1] != "shield")
                    {
                        robot2.takeDamage();
                    }
                }
                break;
            case "reload":
                if (robot1.reload())
                    ReloadObject1.SetActive(true);
                break;
            case "shield":
                ShieldObject1.SetActive(true);
                break;
        }
        switch (action[1])
        {
            case "attack":
                if (robot2.fire()) {
                    BulletObject2.SetActive(true);
                    if (action[0] != "shield")
                    {
                        robot1.takeDamage();
                    }
                }
                break;
            case "reload":
                if (robot2.reload())
                    ReloadObject2.SetActive(true);
                break;
            case "shield":
                ShieldObject2.SetActive(true);
                break;
        }
        if (robot1.dead || robot2.dead)
            state = State.End;
    }

    // Use this for initialization
    void Start() {

        action = new string[2];

        robot1 = new Robot(1);
        robot2 = new Robot(2);

        Player1 = GameObject.FindGameObjectWithTag("Player1");
        Player2 = GameObject.FindGameObjectWithTag("Player2");

        Reload1 = GameObject.FindGameObjectWithTag("R1");
        Reload2 = GameObject.FindGameObjectWithTag("R2");

        Attack1 = GameObject.FindGameObjectWithTag("A1");
        Attack2 = GameObject.FindGameObjectWithTag("A2");

        Shield1 = GameObject.FindGameObjectWithTag("S1");
        Shield2 = GameObject.FindGameObjectWithTag("S2");

        ShieldObject1 = GameObject.FindGameObjectWithTag("Shield1");
        ShieldObject2 = GameObject.FindGameObjectWithTag("Shield2");

        ReloadObject1 = GameObject.FindGameObjectWithTag("Reload1");
        ReloadObject2 = GameObject.FindGameObjectWithTag("Reload2");

        BulletObject1 = GameObject.FindGameObjectWithTag("Bullet1");
        BulletObject2 = GameObject.FindGameObjectWithTag("Bullet2");

        MeshPlayer1 = Player1.GetComponent<MeshRenderer>();
        MeshPlayer2 = Player2.GetComponent<MeshRenderer>();

        MeshReload1 = Reload1.GetComponent<MeshRenderer>();
        MeshReload2 = Reload2.GetComponent<MeshRenderer>();

        MeshAttack1 = Attack1.GetComponent<MeshRenderer>();
        MeshAttack2 = Attack2.GetComponent<MeshRenderer>();

        MeshShield1 = Shield1.GetComponent<MeshRenderer>();
        MeshShield2 = Shield2.GetComponent<MeshRenderer>();

        ReloadObject1.SetActive(false);
        ReloadObject2.SetActive(false);
        ShieldObject1.SetActive(false);
        ShieldObject2.SetActive(false);
        BulletObject1.SetActive(false);
        BulletObject2.SetActive(false);
    }

    // Update is called once per frame

    void Update () {
        if (BulletObject1.activeSelf)
        {
            float step = speed * Time.deltaTime;
            BulletObject1.transform.position = Vector3.MoveTowards(BulletObject1.transform.position, Player2.transform.position, step);
        }

        if (BulletObject2.activeSelf)
        {
            float step = speed * Time.deltaTime;
            BulletObject2.transform.position = Vector3.MoveTowards(BulletObject2.transform.position, Player1.transform.position, step);
        }

        if (state == State.End)
        {
            if (robot1.dead && robot2.dead)
            {
                StateText.text = "Draw";
            }
            else if (robot1.dead)
            {
                StateText.text = "Player 2 Wins";
            }
            else if (robot2.dead)
            {
                StateText.text = "Player 1 Wins";
            }

            if (Time.time - firstTime > waitingTime)
            {
                Application.LoadLevel(0);
            }
        }
        else if (state == State.Wait)
        {
            if (Time.time - firstTime > waitingTime)
            {
                state = State.Ready;

                if (action[0] == "reload")
                    ReloadObject1.SetActive(false);
                else if (action[0] == "attack")
                {
                    BulletObject1.SetActive(false);
                    BulletObject1.transform.position = Player1.transform.position;
                }
                else if (action[0] == "shield")
                    ShieldObject1.SetActive(false);

                if (action[1] == "reload")
                    ReloadObject2.SetActive(false);
                else if (action[1] == "attack")
                {
                    BulletObject2.SetActive(false);
                    BulletObject2.transform.position = Player2.transform.position;
                }
                else if (action[1] == "shield")
                    ShieldObject2.SetActive(false);
            }

        }
        else if (state == State.Ready && (MeshAttack1.isVisible || MeshReload1.isVisible || MeshShield1.isVisible))
        {
            StateText.text = "Player 1 Selected Their Action";

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

            StateText.text = "Player 2 Selected Their Action";

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
            StateText.text = "Fighting";
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
        else if (state == State.Player1)
        {

        }
        else if (state == State.Player2)
        {

        }
        else if (state == State.Fight)
        {
            StateText.text = "Fighting";
            state = State.Wait;
            ResolveFight();
            firstTime = Time.time;
        }

        else if (!MeshPlayer2.isVisible && !MeshPlayer1.isVisible)
        {
            state = State.Waiting2;
            StateText.text = "Waiting for 2 Players";
        }

        else if (MeshPlayer1.isVisible ^ MeshPlayer2.isVisible)
        {
            state = State.Waiting1;
            StateText.text = "Waiting for 1 Player";
        }
        else
        {
            state = State.Ready;
            StateText.text = "Select Players Actions";
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
        bullets = 0;
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

    public bool reload() {
        if (bullets < 3)
        {
            bullets++;
            Bullets[bullets - 1].SetActive(true);
            return true;
        }
        return false;
    }

    public void takeDamage()
    {
        health--;
        HealthBar[health].SetActive(false);
        if (health == 0)
            dead=true;
    }

    public bool fire() {
        if (bullets > 0) {
            bullets--;
            Bullets[bullets].SetActive(false);
            return true;
        }
        return false; 
    }
}