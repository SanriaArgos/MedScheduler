"use client";

import { useState, useEffect } from "react";
import { useRouter } from "next/navigation";
import { formatPhoneDisplay, formatPhoneForAPI, validatePhone } from '@/utils/phoneFormatter';

export default function DoctorProfile() {
    const [doctorData, setDoctorData] = useState(null);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState(null);
    const [successMessage, setSuccessMessage] = useState(null);    const [editData, setEditData] = useState({
        lastName: "",
        firstName: "",
        patronymic: "",
        phone: "",
        currentPassword: "",
        newPassword: "",
        confirmNewPassword: "",
        specialty: "",
        experience: "",
        description: "",
    });
    const router = useRouter();    useEffect(() => {
        const fetchDoctorData = async () => {
            setLoading(true);
            try {
                const userId = localStorage.getItem("userId");
                const response = await fetch(`https://api.medscheduler.ru/get_profile_by_id?user_id=${userId}`);

                const data = await response.json();                if (data.success) {
                    const userData = data.user;
                    const userDataToSet = {
                        userId: userData.user_id || userId,
                        lastName: userData.last_name,
                        firstName: userData.first_name,
                        patronymic: userData.patronymic,
                        phone: userData.phone,
                        specialty: "", // Будет загружено отдельно
                        experience: "", // Будет загружено отдельно
                        description: "", // Будет загружено отдельно
                    };

                    // Установка данных врача в состояние
                    setDoctorData(userDataToSet);
                    setEditData({
                        lastName: userDataToSet.lastName || "",
                        firstName: userDataToSet.firstName || "",
                        patronymic: userDataToSet.patronymic || "",
                        phone: userDataToSet.phone || "",
                        newPassword: "",
                        confirmNewPassword: "",
                        specialty: userDataToSet.specialty || "",
                        experience: userDataToSet.experience || "",
                        description: userDataToSet.description || "",
                    });
                } else {
                    setError(data.message || "Не удалось загрузить данные профиля.");
                }
            } catch (err) {
                console.error(err);
                setError("Ошибка при загрузке данных профиля.");
            } finally {
                setLoading(false);
            }
        };

        fetchDoctorData();
    }, []);

    const handleEditChange = (e) => {
        const { name, value } = e.target;
        setEditData((prev) => ({ ...prev, [name]: value }));
    };

    const handlePhoneChange = (e) => {
        setEditData((prev) => ({ ...prev, phone: e.target.value }));
    };
    
    const handleSaveChanges = async () => {
        setLoading(true);
        setError(null);
        setSuccessMessage(null);

        if (editData.phone && !validatePhone(editData.phone)) {
            setError("Введите корректный  номер телефона.");
            setLoading(false);
            return;
        }        if (editData.newPassword && editData.newPassword !== editData.confirmNewPassword) {
            setError("Новые пароли не совпадают.");
            setLoading(false);
            return;
        }

        // Проверяем, что введен текущий пароль для любых изменений
        if (!editData.currentPassword) {
            setError("Введите текущий пароль для подтверждения изменений.");
            setLoading(false);
            return;
        }const payload = {
            user_id: doctorData.userId,
            current_password: editData.currentPassword,
            last_name: editData.lastName,
            first_name: editData.firstName,
            patronymic: editData.patronymic,
            phone: editData.phone ? formatPhoneForAPI(editData.phone) : undefined,
            specialty: editData.specialty,
            experience: editData.experience,
            description: editData.description,
        };
        if (editData.newPassword) {
            payload.new_password = editData.newPassword;
            payload.new_password_repeat = editData.newPassword;
        }

        try {
            const response = await fetch("https://api.medscheduler.ru/edit_doctor_profile", {
                method: "PATCH",
                headers: {
                    "Content-Type": "application/json",
                },
                body: JSON.stringify(payload),
            });

            const data = await response.json();            if (data.success) {
                setSuccessMessage("Данные профиля успешно обновлены.");
                // Обновление данных врача в состоянии
                setDoctorData((prev) => ({
                    ...prev,
                    lastName: editData.lastName,
                    firstName: editData.firstName,
                    patronymic: editData.patronymic,
                    phone: editData.phone,
                    specialty: editData.specialty,
                    experience: editData.experience,
                    description: editData.description,
                }));

                // Обновляем в localStorage, если телефон изменился
                if (editData.phone !== doctorData.phone) {
                    const userDataFromStorage = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');
                    userDataFromStorage.phone = editData.phone;
                    localStorage.setItem('medSchedulerUser', JSON.stringify(userDataFromStorage));
                }
            } else {
                setError(data.message || "Не удалось обновить данные профиля.");
            }
        } catch (err) {
            console.error(err);
            setError("Ошибка при обновлении данных профиля.");
        } finally {
            setLoading(false);
        }
    };

    if (loading) {
        return <div>Загрузка...</div>;
    }

    return (
        <div className="max-w-3xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Профиль врача</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}

            {successMessage && (
                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                    {successMessage}
                </div>
            )}

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">
                <div className="grid grid-cols-1 md:grid-cols-2 gap-4 mb-4">
                    <div>
                        <label htmlFor="lastName" className="block text-sm font-medium text-gray-700 mb-1">
                            Фамилия
                        </label>
                        <input
                            type="text"
                            name="lastName"
                            id="lastName"
                            value={editData.lastName}
                            onChange={handleEditChange}
                            className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 sm:text-sm"
                            required
                        />
                    </div>
                    <div>
                        <label htmlFor="firstName" className="block text-sm font-medium text-gray-700 mb-1">
                            Имя
                        </label>
                        <input
                            type="text"
                            name="firstName"
                            id="firstName"
                            value={editData.firstName}
                            onChange={handleEditChange}
                            className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 sm:text-sm"
                            required
                        />
                    </div>
                    <div>
                        <label htmlFor="patronymic" className="block text-sm font-medium text-gray-700 mb-1">
                            Отчество (необязательно)
                        </label>
                        <input
                            type="text"
                            name="patronymic"
                            id="patronymic"
                            value={editData.patronymic}
                            onChange={handleEditChange}
                            className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 sm:text-sm"
                        />
                    </div>
                    <div>
                        <label htmlFor="phone" className="block text-sm font-medium text-gray-700 mb-1">
                            Номер телефона
                        </label>
                        <input
                            type="tel"
                            name="phone"
                            id="phone"
                            value={formatPhoneDisplay(editData.phone)}
                            onChange={handlePhoneChange}
                            className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 sm:text-sm"
                            placeholder="+7 (999) 123-45-67"
                        />
                    </div>
                    <div>
                        <label htmlFor="specialty" className="block text-sm font-medium text-gray-700 mb-1">
                            Специальность
                        </label>
                        <input
                            type="text"
                            name="specialty"
                            id="specialty"
                            value={editData.specialty}
                            onChange={handleEditChange}
                            className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 sm:text-sm"
                            required
                        />
                    </div>
                    <div>
                        <label htmlFor="experience" className="block text-sm font-medium text-gray-700 mb-1">
                            Опыт работы (лет)
                        </label>
                        <input
                            type="number"
                            name="experience"
                            id="experience"
                            value={editData.experience}
                            onChange={handleEditChange}
                            min="0"
                            className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 sm:text-sm"
                            required
                        />
                    </div>
                </div>                <div className="mt-4 mb-6">
                    <h3 className="text-lg font-medium mb-2">Подтверждение и изменение пароля</h3>
                    <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                        <div className="md:col-span-2">
                            <label htmlFor="currentPassword" className="block text-sm font-medium text-gray-700 mb-1">
                                Текущий пароль *
                            </label>
                            <input
                                type="password"
                                name="currentPassword"
                                id="currentPassword"
                                value={editData.currentPassword}
                                onChange={handleEditChange}
                                className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 sm:text-sm"
                                required
                            />
                        </div>
                        <div className="md:col-span-2">
                            <label htmlFor="newPassword" className="block text-sm font-medium text-gray-700 mb-1">
                                Новый пароль (необязательно)
                            </label>
                            <input
                                type="password"
                                name="newPassword"
                                id="newPassword"
                                value={editData.newPassword}
                                onChange={handleEditChange}
                                className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 sm:text-sm"
                            />
                        </div>
                        <div className="md:col-span-2">
                            <label htmlFor="confirmNewPassword" className="block text-sm font-medium text-gray-700 mb-1">
                                Повторите новый пароль
                            </label>
                            <input
                                type="password"
                                name="confirmNewPassword"
                                id="confirmNewPassword"
                                value={editData.confirmNewPassword}
                                onChange={handleEditChange}
                                className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 sm:text-sm"
                            />
                        </div>
                    </div>
                </div>

                <div className="flex gap-3 mt-6">
                    <button
                        onClick={handleSaveChanges}
                        className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                    >
                        Сохранить изменения
                    </button>
                    <button
                        onClick={() => router.push("/")}
                        className="px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                    >
                        Отмена
                    </button>
                </div>
            </div>
        </div>
    );
}
