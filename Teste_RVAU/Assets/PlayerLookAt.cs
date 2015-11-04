using UnityEngine;
using System.Collections;

public class PlayerLookAt : MonoBehaviour
{
    public Transform target;

    void Update()
    {
        if (target.GetComponent<MeshRenderer>().isVisible)
        {
            transform.LookAt(target);
           // transform.Rotate(new Vector3(0, -90, 0));
        }
    }
}