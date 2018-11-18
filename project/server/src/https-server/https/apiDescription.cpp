#include "SecureRestApi.hpp"

using namespace elevation;

void SecureRestApi::createSecureDescription_() {
    auto superviseurPath = m_desc.path("/superviseur");

    /**
     * File Management
     */
    superviseurPath
            .route(m_desc.get("/file"))
            .bind(&SecureRestApi::getSuperviseurFile_, this)
            .hide();

    superviseurPath
            .route(m_desc.del("/chanson/:no"))
            .bind(&SecureRestApi::deleteSuperviseurChanson_, this)
            .hide();

    superviseurPath
            .route(m_desc.post("/inversion"))
            .bind(&SecureRestApi::postSuperviseurInversion_, this)
            .hide();

    /**
     * Volume Management
     */
    createVolumeDescription_(m_desc);

    /**
     * Statistics
     */
    superviseurPath
            .route(m_desc.get("/statistiques"))
            .bind(&SecureRestApi::getSuperviseurStatistiques_, this)
            .hide();

    /**
     * Blacklist management
     */
    superviseurPath
            .route(m_desc.post("/bloquer"))
            .bind(&SecureRestApi::postSuperviseurBloquer_, this)
            .hide();

    superviseurPath
            .route(m_desc.post("/debloquer"))
            .bind(&SecureRestApi::postSuperviseurDebloquer_, this)
            .hide();

    superviseurPath
            .route(m_desc.get("/listenoire"))
            .bind(&SecureRestApi::getSuperviseurListenoire_, this)
            .hide();

    /**
     * Login Management
     */
    superviseurPath
            .route(m_desc.post("/login"))
            .bind(&SecureRestApi::superviseurLogin_, this)
            .hide();

    superviseurPath
            .route(m_desc.post("/logout"))
            .bind(&SecureRestApi::superviseurLogout_, this)
            .hide();

    superviseurPath
            .route(m_desc.post("/changement_mdp"))
            .bind(&SecureRestApi::postChangePassword_, this)
            .hide();
}

