using UnityEngine;
using System.Collections;

public class HealthController : MonoBehaviour {

    public float max_health = 100f;
    public float current_health = 0f;
    public GameObject healthBar;
    public GameObject allBar;

    // Use this for initialization
    void Start () {
        current_health = max_health;
        //InvokeRepeating("decreaseHealth", 1f, 2f);
	}
	
	// Update is called once per frame
	void Update () {
        if (!GetComponent<MeshRenderer>().isVisible){
            allBar.SetActiveRecursively(false);
        }
        else
        {
            allBar.SetActiveRecursively(true);
        }
	}

    void decreaseHealth()
    {
        current_health -= 2f;
        float calc_health = current_health / max_health;
        setHealthBar(calc_health);
    }

    public void setHealthBar(float myHealth){
        healthBar.transform.localScale = new Vector3(myHealth, healthBar.transform.localScale.y, healthBar.transform.localScale.z);
    }
}
