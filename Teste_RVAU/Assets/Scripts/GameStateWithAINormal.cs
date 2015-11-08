﻿using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
using System;

public class GameStateWithAINormal : MonoBehaviour {

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

    public enum State { Waiting2, Waiting1, Ready, Player1, Fight, Wait, End}
    public static State state;
    public string[] action;
    public Text StateText;

    public string calculateAction() {
        List<string> actions = new List<string>();

        if (robot1.bullets != 0)
            actions.Add("shield");
        if (robot2.bullets != 0)
            actions.Add("attack");
        if (robot2.bullets != 3)
            actions.Add("reload");

        return actions[new System.Random().Next(0,actions.Count)];
    }

    public void ResolveFight() {
        switch (action[0])
        {
            case "attack":
                if (robot1.fire() )
                {
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
;
        Attack1 = GameObject.FindGameObjectWithTag("A1");

        Shield1 = GameObject.FindGameObjectWithTag("S1");

        ShieldObject1 = GameObject.FindGameObjectWithTag("Shield1");
        ShieldObject2 = GameObject.FindGameObjectWithTag("Shield2");

        ReloadObject1 = GameObject.FindGameObjectWithTag("Reload1");
        ReloadObject2 = GameObject.FindGameObjectWithTag("Reload2");

        MeshPlayer1 = Player1.GetComponent<MeshRenderer>();
        MeshPlayer2 = Player2.GetComponent<MeshRenderer>();

        MeshReload1 = Reload1.GetComponent<MeshRenderer>();

        MeshAttack1 = Attack1.GetComponent<MeshRenderer>();

        MeshShield1 = Shield1.GetComponent<MeshRenderer>();

        ReloadObject1.SetActive(false);
        ReloadObject2.SetActive(false);
        ShieldObject1.SetActive(false);
        ShieldObject2.SetActive(false);
    }

    // Update is called once per frame

    void Update () {

        if (state == State.End) {
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
        }
        else if (state == State.Wait){
            if (Time.time - firstTime > 1)
            {
                state = State.Ready;

                if (action[0] == "reload")
                    ReloadObject1.SetActive(false);
                else if (action[0] == "attack")
                {

                }
                else if (action[0] == "shield")
                    ShieldObject1.SetActive(false);

                if (action[1] == "reload")
                    ReloadObject2.SetActive(false);
                else if (action[1] == "attack")
                {

                }
                else if (action[1] == "shield")
                    ShieldObject2.SetActive(false);
            }
                        
        }
        else if (state == State.Ready && (MeshAttack1.isVisible || MeshReload1.isVisible || MeshShield1.isVisible))
        {
            StateText.text = "Player 1 Selected Their Action";

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

            action[1] = calculateAction();

        }
        else if (state == State.Player1)
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