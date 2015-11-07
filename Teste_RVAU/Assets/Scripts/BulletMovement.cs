using UnityEngine;
using System.Collections;

public class BulletMovement : MonoBehaviour {

    public Transform target;
    public Transform position;
    public float speed;

    void Start() {}

    void Update()
    {
        /*if (target.GetComponent<MeshRenderer>().isVisible)
        {
            float step = speed * Time.deltaTime;
            position.position = Vector3.MoveTowards(position.position, target.position, step);
            if (position.position.Equals(target.position))
                Destroy(this.gameObject);
        }*/
    }

    public BulletMovement(Transform position, Transform target, float speed) {
        this.position = position;
        this.target = target;
        this.speed = speed;
    }

}